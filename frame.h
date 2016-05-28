#include <stdbool.h>
#ifndef frame_h
#define frame_h

typedef struct{
    int pid;
    int page;
		bool used;
}frame;

void initializeFrame(frame *f);
int getFramePid(frame *f);
void setFramePid(frame *f, int pid);
int getFramePage(frame *f);
void setFramePage(frame *f, int page);
void setFrameUsed(frame *f, bool u);
bool getFrameUsed(frame *f);

#endif
