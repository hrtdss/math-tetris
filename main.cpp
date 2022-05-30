#include <time.h>
#include <stdbool.h>
#include "graphics.h"

#define WIDTH  900 // ������  ���� ���������� � ��������
#define HEIGHT 600 //  ������  ���� ���������� � ��������
#define AMOUNTOFLINES 6 // ������ �������� ���� � �������
#define AMOUNTOFCOLUMNS 5 // ������ �������� ���� � �������
#define EDGELENGTH 85 // ����� ����� ������
#define AMOUNTOFDATA 4 // ��������� �������� � ������� colors
#define AMOUNTOFCOLORS 11 // ���������� ����� ����� (�������) �, ��������������, �� �����

int colors[AMOUNTOFCOLORS][AMOUNTOFDATA]; // �������, �������� ����� � ������ � �������������� �� ����
int board[AMOUNTOFLINES][AMOUNTOFCOLUMNS]; // ������, �������������� ������� ����
int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1}; // ������� �������
int topLeftX = 110, topLeftY = 35; // ���������� ������ ���� �������� ����
char buff[10010]; // �����
_Bool gameContinuing; // ��������� ���� (������������ �� ���)

void loadData(); // �������� ����������� ������ � �����������
void initButtons(); // ����������� ������(���������) ������
int menuButtons(int currStatus); // ����� ������ ����
void showRules(); // ������������ �� ���� � ��������� ����
void showAbout(); // ������������ �� ���� � ����������� � ����
void addNumber(); // ���������� ����� ������ �� ������� ����
void initNewGame(); // ������������� ����
void movingCell(int dir); // �������� ����
void drawCells(); // ��������� ����� �������� ����

int main() {
   srand(time(0));
   initwindow(WIDTH, HEIGHT, "�������������� ������");
   setbkcolor(LIGHTGRAY);
   clearviewport();
   
   loadData();
   initNewGame();
   drawCells();
   
   while(gameContinuing) {
      char ch = getch();
      
      delay(100); 
      
      if (ch == 78 || ch == -5) // down
         movingCell(1);

      if (ch == 74 || ch == -12) // left
         movingCell(2);

      if (ch == 76 || ch == -30) // right
         movingCell(3);
      
      if (ch == 110 || ch == -14) // new game
         initNewGame();
      
      if (ch == 27) { // esc
         gameContinuing = 0; 
         break; 
      }
      
      drawCells();
   }

   closegraph();
}

void loadData() {
   FILE *fin = fopen("data.txt", "r");

   for (int i = 0; i < AMOUNTOFCOLORS; i++)
      for (int j = 0; j < AMOUNTOFDATA; j++)
         fscanf(fin, "%d", &colors[i][j]);

   fclose(fin); 
}

void addNumber() {
   _Bool isCellEngaged = 1;
   int x = 0, y;
   
   while (isCellEngaged) {
      y = rand() % AMOUNTOFCOLUMNS;
      
      if (board[x][y] == 0)
         isCellEngaged = 0;
   }
   
   board[x][y] = 2;
}

void initNewGame() {
   settextjustify(0, 2);
   settextstyle(4, HORIZ_DIR, 3);
   
   gameContinuing = 1;
   
   for (int i = 0; i < AMOUNTOFLINES; i++)
      for (int j = 0; j < AMOUNTOFCOLUMNS; j++)
         board[i][j] = 0;
   
   addNumber();
}

_Bool isPossible(int i, int j, int nextI, int nextJ) {
   if (nextI < 0 || nextJ < 0 || nextI >= AMOUNTOFLINES || nextJ >= AMOUNTOFCOLUMNS || (board[i][j] != board[nextI][nextJ] && board[nextI][nextJ] != 0))
      return false;
   
   return true;
}

void movingCell(int dir) {
   int startX = 0, startY = 0;
   int xStep = 1, yStep = 1;
   
   if (dir == 1) {
      xStep = -1;
      startX = AMOUNTOFLINES - 1;
   } 
   if (dir == 3) {
      yStep = -1;
      startY = AMOUNTOFCOLUMNS - 1;
   }
   
   _Bool isFreeSpace = 0;
   for (int i = startX; i >= 0 && i < AMOUNTOFLINES; i += xStep)
      for (int j = startY; j >= 0 && j < AMOUNTOFCOLUMNS; j += yStep) {
         if (board[i][j] == colors[AMOUNTOFCOLORS - 1][0]) { // �������� � ������ ��������� ���� (������� ����� ������������ �����)
            initNewGame();
            return;
         }
         
         int nextX = i + dx[dir], nextY = j + dy[dir];
         if (isPossible(i, j, nextX, nextY)) {
            board[nextX][nextY] += board[i][j];
            board[i][j] = 0;
            isFreeSpace = 1;
         }
      }
   
   if (isFreeSpace)
      addNumber();
}

void drawCells() {
   int x, y;
   int color;
   
   for (int i = 0; i < AMOUNTOFLINES; i++) {
      for (int j = 0; j < AMOUNTOFCOLUMNS; j++) {
         x = topLeftX + j * (EDGELENGTH),
         y = topLeftY + i * (EDGELENGTH);
         
         for (int k = 0; k < 11; k++)
            if (board[i][j] == colors[k][0]) // ����� ����� �� �����, ������������ � ������
               color = COLOR(colors[k][1], colors[k][2], colors[k][3]);
         
         setcolor(color);
         setfillstyle(1, color);
         setbkcolor(color);
         bar(x, y, x + EDGELENGTH, y + EDGELENGTH);
         
         setcolor(BLACK); 
         if (board[i][j] != 0) { // ������������� ���������� ����� �� ���-�� ���� � ���
            snprintf(buff, 10010, "%d", board[i][j]);
            
            if (board[i][j] >= 0 && board[i][j] <= 9)
               outtextxy(x + 5 + EDGELENGTH / 3, y + EDGELENGTH / 2, buff);
            if (board[i][j] >= 10 && board[i][j] <= 99)
               outtextxy(x + 3 + EDGELENGTH / 4, y + EDGELENGTH / 2, buff);
            if (board[i][j] >= 100 && board[i][j] <= 999)
               outtextxy(x - 3 + EDGELENGTH / 5, y + EDGELENGTH / 2, buff);
            if (board[i][j] >= 1000)
               outtextxy(x + 3, y + EDGELENGTH / 2, buff);
         }
         
         topLeftX += 5;
      }
      topLeftY += 5;
      topLeftX -= 5 * AMOUNTOFCOLUMNS;
   }
   
   topLeftY -= 5 * AMOUNTOFLINES;
}