// vim: set noet ts=4 sw=4:

#include "machine/cgascr.h"
// Verwendung der Klasse IO_Port für Zugriff auf die Register
#include "machine/io_port.h"

char* CGA_Screen::screen = (char*)0xb8000;
IO_Port i_port(0x3d4);
IO_Port d_port(0x3d5);

CGA_Screen::CGA_Screen (int from_col, int to_col, int from_row, int to_row, bool use_cursor) 
    : from_col(from_col), to_col(to_col), from_row(from_row), to_row(to_row), use_cursor(use_cursor)
    {      
        width = to_col - from_col+1;
        height = to_row - from_row+1;
        reset();
        /*rel_x = 0;
        rel_y = 0;*/
    }

void CGA_Screen::setpos (int x, int y){

    x = 20;
    y = 20;

    int total_x = from_col + x;
    int total_y = from_row + y;

    if(use_cursor){
        uint16_t data = (uint16_t)(total_x + total_y*80);
        i_port.outb(0xe);
        d_port.outb((uint8_t)((data & 0xFF00) >> 8));
        i_port.outb(0xf);
        d_port.outb((uint8_t)data & 0x00FF);
    }
    rel_x = x;
    rel_y = y;
    
}

void CGA_Screen::getpos (int& x, int& y){

    if(use_cursor){
        uint16_t data = 0;
        i_port.outb(0xe);
        data = 0xFF00 & (d_port.inb() << 8);
        i_port.outb(0xf);
        data += 0xFF & d_port.inb();
        y = from_row + (data / 80);
        x = from_col + (data - y*80);
    } else {
        x = rel_x;
        y = rel_y;
    }
}


void CGA_Screen::print (char* string, int length, Attribute attrib){

    int i = 0; 

    while (i < length){
        
        //shift text
        if ((rel_x == width || string[i] == '\n') && rel_y == height-1){

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


        if(string[i] == '\0'){
            ++i;
        }
        else if(string[i] == '\n'){
            ++rel_y;
            rel_x = 0;
            ++i;
            
        }else{
            show(from_col + rel_x, from_row + rel_y, string[i], attrib);
        
            rel_x++;
            i++;
        }
        //setpos(rel_x, rel_y);

    }

}


void CGA_Screen::reset (char character, Attribute attrib){

    for(int x = from_col; x <= to_col; ++x){
        for(int y = from_row; y <= to_row; ++y){
            show(x, y, character, attrib);
        }
    }
    //setpos(0,0);

}

 void CGA_Screen::show (int x, int y, char character, Attribute attrib){
    
    if(x < 0) x = 80+x;
    if(y < 0) y = 25+y;

    if(x < 0 || x > 79 || y < 0 || y > 24) return;

    screen[(x + y*80)*2] = character;
    screen[(x + y*80)*2 + 1] = *((char*) &attrib);
    
}
