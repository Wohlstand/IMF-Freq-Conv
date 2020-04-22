/*
 * IMF frequency converter - A tiny tool which converts a chunk delays
 * in the IMF (id Software Music Format) between 280Hz, 560hz and 700Hz tempo speeds.
 *
 * Copyright (c) 2016 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

int processFile(char* src, char*dst, double factor)
{
    unsigned char null[2]  = {0, 0};
    unsigned char byte2[2] = {0, 0};
    unsigned char entry[4] = {0, 0, 0, 0};

    FILE* source = fopen(src, "rb");
    if(!source)
    {
        printf("FAIL TO OPEN SOURCE FILE!\n(%s)\n\n", src);
        return 3;
    }

    FILE* target = fopen(dst, "wb");
    if(!target)
    {
        printf("FAIL TO OPEN TARGET FILE!\n(%s)\n\n", dst);
        return 3;
    }

    if(fread(byte2, 1, 2, source)!=2)
    {
        printf("FAIL TO READ SOURCE FILE!\n(%s)\n\n", src);
        return 3;
    }
    unsigned int lenght = 999999999;
    if( memcmp(byte2, null, 2)==0 )
    {
        fseek(source, 0, SEEK_END);
        lenght = ftell(source);
        printf("Lenght is %d (detected)\n", lenght);
        fseek(source, 0, SEEK_SET);
        byte2[0] = (unsigned char)(lenght&0x0000FF);
        byte2[1] = (unsigned char)((lenght&0x00FF00)>>8);
        fwrite(byte2, 1, 2, target); //Store lenght byte!
    }
    else
    {
        lenght  = ((unsigned short)byte2[0]);
        lenght |= ((unsigned short)byte2[1])<<8;
        printf("Lenght is %d (got from file)\n", lenght);
        fwrite(byte2, 1, 2, target); //Just copy-paste as-is
    }

    unsigned int got = 0;
    while( (fread(entry, 1, 4, source)==4) && (got<lenght) )
    {
        unsigned short len = 0;
        len  = ((unsigned short)entry[2]);
        len |= ((unsigned short)entry[3])<<8;
        double lenT = len;
        lenT = lenT*factor;//Change delay with applying of the factor
        len = (unsigned short)lenT;
        entry[2] = (unsigned char)((len&0x00FF));
        entry[3] = (unsigned char)((len&0xFF00)>>8);
        fwrite(entry, 1, 4, target);//Write modified entry into the new file
        got += 4;
    }

    unsigned char onebyte = 0;
    //Copy-Paste bytes left in the tail
    while(fread(&onebyte, 1, 1, source)==1)
    {
        fwrite(&onebyte, 1, 1, target);//Write all tail bytes as-is (tags, etc.)
    }

    fclose(source);
    fclose(target);

    printf("DONE %iu bytes!\n\n"
           "------------------------------------------------------------------------------\n", got);

    return 0;
}

const char *logo  =    "------------------------------------------------------------------------------\n"
                       "IMF frequency converter - A tiny tool which converts a chunk delays in the \n"
                       "IMF (id Software Music Format) between 280Hz, 560hz and 700Hz tempo speeds.\n\n"

                       "Licensed under GNU General Public License version 3.\n"
                       "Source code: https://github.com/Wohlstand/IMF-Freq-Conv\n"
                       "------------------------------------------------------------------------------\n";

const char *usage =    "------------------------------------------------------------------------------\n"
                       "Syntax: imfreqc <SourceFrequency> <oldfile>.imf <DestFrequency> <newfile>.imf\n\n"
                       "SourceFrequency - number 280 560 or 700\n"
                       "DestFrequency   - number 280 560 or 700\n\n"

                       "------------------------------------------------------------------------------\n"
                       "Supported frequencies:\n"
                       "------------------------------------------------------------------------------\n"
                       "  280Hz - Duke Nukem II\n"
                       "  560Hz - Bio Menace, Commander Keen, Cosmo's Cosmic Adventures, Monster Bash\n"
                       "  700Hz - Blake Stone, Major Stryker, Operation Body Count, Wolfenstein 3-D\n\n"
                       "------------------------------------------------------------------------------\n";



int main(int argc, char**argv)
{
    printf("%s\n", logo);

    if(argc<5)
    {
        if( (argc<2) || (argc>=2)&&(strcmp(argv[1], "--help")!=0) )
            printf("                      TOO FEW ARGUMENTS!\n\n");
        printf("%s", usage);
        return 1;
    }

    double sourceF = (double)atoi(argv[1]);
    double targetF = (double)atoi(argv[3]);

    if((sourceF != 280.0) && (sourceF != 560.0) && (sourceF != 700.0) )
    {
        printf("                     UNSUPPORTED SOURCE FREQUENCY!\n\n%s", usage);
        return 1;
    }

    if((targetF != 280.0) && (targetF != 560.0) && (targetF != 700.0) )
    {
        printf("                     UNSUPPORTED TARGET FREQUENCY!\n\n%s", usage);
        return 1;
    }

    if(sourceF==targetF)
    {
        printf("FREQUENCIES ARE SAME, Fixing lenght bytes only!\n");
    }

    double factor = targetF/sourceF;

    return processFile(argv[2], argv[4], factor);
}

