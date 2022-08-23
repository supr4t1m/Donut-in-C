# Donut-in-C

This is a code inspired by the blog of **Andy Sloane** on [Donut math: how donut.c works](https://www.a1k0n.net/2011/07/20/donut-math.html)
I have simply tried to code the program and  understand the bits as far as possible by myself. 

### Windows
replace the ```printf("\x1b[H")``` with the ```gotoxy(0, 0)```

the function ```gotoxy()``` to move the cursor is defined below
```
void gotoxy(int x, int y) {
    COORD c = { .X = x, .Y = y };
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(handle, c);
}
```

### Update
This repository now contains code for spinning cube also. 
