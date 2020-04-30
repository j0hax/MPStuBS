// vim: set noet ts=4 sw=4:

#include "machine/cgascr.h"
// Verwendung der Klasse IO_Port für Zugriff auf die Register
#include "machine/io_port.h"

char* CGA_Screen::screen = (char*)0xb8000;

CGA_Screen::CGA_Screen (int from_col, int to_col, int from_row, int to_row, bool use_cursor) 
    : from_col(from_col), to_col(to_col), from_row(from_row), to_row(to_row), use_cursor(use_cursor)
    {      
        width = to_col - from_col+1;
        height = to_row - from_row+1;
        reset();
        /*rel_x = 0;
        rel_y = 0;*/
    }

void CGA_Screen::print (char* string, int length, Attribute attrib){

    int i = 0; 

    while (i < length){
        
        //shift text
        if (rel_x == width && rel_y == height-1){

            for(int y = from_row; y < to_row; ++y){
                for(int x = from_col; x <= to_col; ++x){
                    
                    screen[(x + y*80)*2] = screen[(x + (y+1)*80)*2];
                    screen[(x + y*80)*2 + 1] = screen[(x + (y+1)*80)*2 + 1];
                }
            }
            rel_x = 0;
            //clear last line
            for(int c = 0; c < width; ++c){
                show(from_col + c, from_row + rel_y, ' ');
            }

        // new line
        } else if (rel_x == width){
            ++rel_y;
            rel_x = 0;
        }

        show(from_col + rel_x, from_row + rel_y, string[i], attrib);
        
        rel_x++;
        i++;
    }

}


void CGA_Screen::reset (char character, Attribute attrib){

    for(int x = from_col; x <= to_col; ++x){
        for(int y = from_row; y <= to_row; ++y){
            show(x, y, character, attrib);
        }
    }
    rel_x = 0;
    rel_y = 0;

}

 void CGA_Screen::show (int x, int y, char character, Attribute attrib){
    
    if(x < 0) x = 80+x;
    if(y < 0) y = 25+y;

    if(x < 0 || x > 79 || y < 0 || y > 24) return;

    screen[(x + y*80)*2] = character;
    screen[(x + y*80)*2 + 1] = *((char*) &attrib);
    
}
