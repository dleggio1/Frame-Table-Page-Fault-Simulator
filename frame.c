#include "frame.h"

void initializeFrame(frame *f){
    f->pid = -1;
    f->page = -1;
		f->used = false;
}

int getFramePid(frame *f){
    return f->pid;
}

void setFramePid(frame *f, int pid){
    f->pid = pid;
}

int getFramePage(frame *f){
    return f->page;
}

void setFramePage(frame *f, int page){
    f->page = page;
}

bool getFrameUsed(frame *f){
		return f->used;
}
void setFrameUsed(frame *f, bool u){
	f->used = u;
}
