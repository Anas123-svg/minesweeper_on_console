#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<iomanip>
#include<fstream>
#include <chrono>
#define BLACK 0
#define BROWN 6
#define WHITE 15
#define GREEN 2
#define RED 4
#define LBLUE 9

#define GOLDEN 88
void SetClr(int tcl, int bcl)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (tcl + (bcl * 16)));
}
void gotorow_col(int rpos, int cpos) {
    COORD scren;
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    scren.X = cpos;
    scren.Y = rpos;
    SetConsoleCursorPosition(output, scren);

 }
struct pos {
    int rpos;
    int cpos;
};
struct cell {
    char value;
    char isOpen;
    char isFlag;
};
void qdd(cell**& mine, char n, char v);
bool getRowColbyClick(int& rpos, int& cpos) {
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    do {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.EventType == MOUSE_EVENT) {
            if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
                rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
                return true; // Left click
            }
            else if (InputRecord.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
                cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
                rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
                return false; // Right click
            }
        }
    } while (true);
}
void getRowColbyLeftClick2(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    } while (true);
}


#define dim 9
#define Now 10
//#define mm 19
const int dx[] = { -1,-1,-1,0,0,1,1,1 };
const int dy[] = { -1,0,1,-1,1,-1,0,1, };
const char dx1[] = { '- 1',' - 1',' - 1','0','0','1','1','1'};
const char dy1[] = { ' - 1','0','1','- 1','1',' - 1','0','1'};


void pm(cell** mine) {
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    srand(time(0));
    char x, y;
    for (char i = 0; i < Now; i++) {
        x = rand()%dim;
        y = rand()%dim;
        if (mine[x][y].value == 0) {
            mine[x][y].value = char(5);
        }
        else
            i--;
    }
    
}
bool isboundry(int pos) {
    return pos >= 0 && pos < dim && pos != -1;
}
void surroudings(cell** &mine, char & count, char & tm, char i, char j) {
    for (char k = 0; k < 8; k++) {
        char xx = i + dx[k];
        char yy = j + dy[k];
        if (xx >= 0 && xx < dim && yy >= 0 && yy < dim) {
            if ((*(*(mine + xx) + yy)).value == char(5)) {
                count++;
                tm++;
            }
        }
    }
}
void calculatemine(cell** mine, char& tm) {
    for (char i = 0; i < dim; i++) {
        for (char j = 0; j < dim; j++) {
            if ((*(*(mine + i) + j)).value == 0) {
                char count = 48;
                surroudings(mine, count, tm, i, j);

                (*(*(mine + i) + j)).value = char(count);
            }
        }
    }
}
void bar(int h, int sy) {
    for (char i = '0'; i < 82; i++) {
        gotorow_col(h, sy);
        SetClr(2, 0);
        std::cout << "" << char(22) << char(22);
        //h++;
        sy++;
    }
}
void floodFill(cell** mine, char startRow, char startCol) {
    if (!isboundry(startRow) || !isboundry(startCol) || mine[startRow][startCol].isOpen) {
        return;
    }

    mine[startRow][startCol].isOpen = '1';
    char r1[100] = {};
    char c2[100] = {};
    char si = '0';
    char ri = '0';

    *(r1+ ri) = startRow;
    *(c2+ ri) = startCol;
    ri++; 

    while (si < ri) {
        
        char currentRow = *(r1+ si);
        char currentCol = *(c2+ si);
        si++;

        for (int k = 0; k < 8; k++) {
            char newRow = currentRow + *(dx+k);
            char newCol = currentCol + *(dy + k);

            if (isboundry(newRow) && isboundry(newCol) && (*(*(mine + newRow) + newCol)).isOpen==0) {
                (*(*(mine + newRow) + newCol)).isOpen = '1';

                if ((*(*(mine + newRow) + newCol)).value == 48) {
                    *(r1+ ri) = newRow;
                    *(c2+ ri) = newCol;
                    ri++;
                }
            }
        }
    }

}
void init(cell**& mine, char&tm) {
    mine = new cell * [dim];
    for (int i = 0; i < dim; i++) {
        mine[i] = new cell[dim]{};
    }
    pm(mine);
    tm = 48;
    calculatemine(mine, tm);
}
void display(cell** mine) {
    gotorow_col(5, 55);
    SetClr(5, 0);
    std::cout << "MineSweeper";
    int f = 10,h=9;
    int ry = 39, sy=43;
    for (char i = '0'; i < '9'; i++) {
        gotorow_col(h-1, sy);
        SetClr(2, 0);
        std::cout << i<<" ";
        //h++;
        sy+=4;
    }
    //bar(h, sy-36);
    sy = 42, h = 9;

    std::cout << std::endl;
    for (int i = 0; i < dim; i++) {
        
        gotorow_col(f, ry);
        SetClr(2, 0);
       //cout << " "<< char(22);
        std::cout << i<<" ";
        SetClr(15, 0);
        for (int j = 0; j < dim; j++) {
            if ((*(*(mine + i) + j)).isOpen=='1' && (*(*(mine + i) + j)).isFlag==0) {
                //&& !mine[i][j].isFlag
                SetClr(9, 0);
                std::cout << std::setw(3) << (*(*(mine + i) + j)).value << " ";
            }
            else if ((*(*(mine + i) + j)).value == char(5) && (*(*(mine + i) + j)).isOpen=='1' && (*(*(mine + i) + j)).isFlag==0) {
                std::cout << (*(*(mine + i) + j)).value;
                break;
            }
            else if ((*(*(mine + i) + j)).isOpen=='0' && (*(*(mine + i) + j)).isFlag=='1') {
                SetClr(4, 0);
                std::cout << std::setw(3) << "F" << " ";
            }
            else {
              //  gotorow_col(10, 10);
                SetClr(15, 0);
                if ((*(*(mine + i) + j)).isFlag=='1') {
                    SetClr(4, 0);

                    std::cout << std::setw(3) << "F"<<" ";
                }
                else {
                    SetClr(15, 0);
                  //  std::cout << std::setw(3) << (*(*(mine + i) + j)).value<<" ";
                    
                    std::cout << std::setw(2)<<"|" << char(-37) << "|";
                   // bar(f + 1, -36);
                    
                }
            }
            if (j == 8) {
                std::cout << std::endl;
            }
            
            
            //gotorow_col(f, 55);
        }
        std::cout << std::endl;
       
        f+=2;
    }
}
void updatedisplay(cell** mine, int n, int v) {
    (*(*(mine + n) + v)).isOpen = '1';
    display(mine);
}
void flag(cell** mine, char n, char v, char& c) {
    //mine[n][v].isFlag = true;
    if ((*(*(mine + n) + v)).isOpen == 0 && (*(*(mine + n) + v)).isFlag == 0 && c<60) {
        (*(*(mine + n) + v)).isFlag = '1';
        (*(*(mine + n) + v)).isOpen = 0;
        c++;
    }
    else if((*(*(mine + n) + v)).isOpen == 0 && (*(*(mine + n) + v)).isFlag == '1') {
        (*(*(mine + n) + v)).isFlag = 0;
        (*(*(mine + n) + v)).isOpen = 0;
        c++;
    }
}
void  unflag(cell** mine, char n, char v, char& c) {
    (*(*(mine + n) + v)).isFlag = '0';
    (*(*(mine + n) + v)).isOpen = '0';
    c--;
}
bool win(cell** mine, char tm) {
    char openedCells = 48;
    for (char i = 0; i < dim; i++) {
        for (char j = 0; j < dim; j++) {
            if ((*(*(mine + i) + j)).isOpen=='1' && (*(*(mine + i) + j)).value != char(5)) {
                openedCells++;
            }
        }
    }
    //102
    return openedCells == char(118);
}
void cf(cell**& mine, char n, char v, char & cf) {
    for (char i = 0; i < dim; i++) {
        for (char j = 0; j < dim; j++) {
            if ((*(*(mine + n) + v)).isOpen == '1') {
                cf = 48;
                for (char o = 0; o < 8; o++) {
                    char xx = n + dx[o];
                    char yy = v + dy[o];
                    if (xx >= 0 && xx < dim && yy >= 0 && yy < dim) {
                        if ((*(*(mine + xx) + yy)).isFlag == '1') {
                            cf++;
                        }
                    }
                }
            }
        }
    }
}
void cm(cell**& mine, char n, char v, char& cf) {
    for (char i = 0; i < dim; i++) {
        for (char j = 0; j < dim; j++) {
            if ((*(*(mine + n) + v)).isOpen == '1') {
                cf = 48;
                for (char o = 0; o < 8; o++) {
                    char xx = n + dx[o];
                    char yy = v + dy[o];
                    if (xx >= 0 && xx < dim && yy >= 0 && yy < dim) {
                        if ((*(*(mine + xx) + yy)).value == char(5)&& (*(*(mine + xx) + yy)).isFlag == 0) {
                            cf++;
                        }
                    }
                }
            }
        }
    }
}
void qdd(cell** &mine, char n, char v) {
    if ((*(*(mine + n) + v)).isOpen == '1') {
        for (char k = 0; k < 8; k++) {
            char xx = n + dx[k];
            char yy = v + dy[k];
            if (xx >= 0 && xx < dim && yy >= 0 && yy < dim) {
                if ((*(*(mine + xx) + yy)).isOpen == 0) {
                    (*(*(mine + xx) + yy)).isOpen = '1';
                }
            }
        }
    }
}
void updatedisplay2(cell** mine, int n, int v) {
    (*(*(mine + n) + v)).isOpen = '1';
   // display(mine);
}
void game(cell** mine, char &winsss, char &losses, char &tgame, char &time) {
    auto start = std::chrono::steady_clock::now();
    char tm = 0;
    tgame++;
    init(mine, tm);
    gotorow_col(8, 45);
    int n, v, f;
    char c = '0', w = '0';
    char cfg = 48,cmg=48;

    while (true) {
        display(mine);
        if (getRowColbyClick(n, v)) {
            gotorow_col(30, 45);
            n = (n - 10), v = abs((v)-43);
            n = n / 2;
            if (v == 0) {
                v = 0;
            }
            else if (v == 4) {
                v = v / 4;
            }
            else if (v >= 8 || v != 13) {
                v = v / 4;
            }
            if (v > 8 || n > 8) {
                continue;
            }


        }
        //if (!getRowColbyClick(n, v))
        else {
            n = (n - 10), v = abs((v)-43);
            n = n / 2;
            if (v == 0) {
                v = 0;
            }
            else if (v == 4) {
                v = v / 4;
            }
            else if (v >= 8 || v != 13) {
                v = v / 4;
            }
            //(*(*(mine + n) + v)).isOpen == 0;
            flag(mine, n, v, c);
            // if ((*(*(mine + n) + v)).isOpen == 0) {
              //   flag(mine, n, v, c);
             //}
            continue;
        }
       // updatedisplay2(mine, n, v);
//        if (mine[n][n].isOpen == '1') {
        
        //}
        cf(mine, n, v, cfg);
        if (cfg == mine[n][v].value) {
            qdd(mine, n, v);
            cm(mine, n, v, cmg);
            if (cmg > 48 ) {
                display(mine);
                break;
            }
        }
        if (n < 0 || n >= 10 || v < 0 || v >= 90 && (n != -5 && v != 3)) {
            continue;
        }
        else if (mine[n][v].value == 48 && mine[n][v].isFlag == 0) {
            floodFill(mine, n, v);
        }

        if (win(mine, tm)) {
            std::cout << "Winner ";
            winsss++;
            break;
        }
        Sleep(200);
        if (mine[n][v].isFlag == 0) {
            updatedisplay(mine, n, v);
        }
        //system("cls");
        if ((*(*(mine + n) + v)).value == char(5) && (*(*(mine + n) + v)).isFlag == 0) {
            //cout << (*(*(mine + n) + v)).value << endl;
            std::cout << "Game Over! You clicked on a mine." << std::endl;
            display(mine);
            losses++;
            break;
        }

    }

    delete[] mine;
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    time  = ((duration.count() / 1000) % 60);
    int mins = time / 60;
}

//using namespace std;

int lenq(char a[],int & l) {
    for (int i = 0; i < 20; i++) {
        l++;
        if (a[i] == 0) {
            l--;
            break;
        }
    }
    return l;
}

struct usdata {
    uint32_t times;
    uint32_t wins;
    uint32_t looses;
    uint32_t tgames;
};
struct user {
    char ID[20];
    char PASS[20];
    usdata rec;
};
struct ud {
    char ID[20];
    char PASS[20];
    usdata rec;


};






void swap(int& a, int& b) {
    int t = a;
    a = b;
    b = t;
}





int main() {
    const int maxNameLength = 20;
    cell** mine = nullptr;
    user us[20];
    ud usd[20];
    ud usd2[20];
    std::fstream file("creds.bin", std::ios::binary | std::ios::in | std::ios::out | std::ios::app);
    std::ofstream ff("creds.bin", std::ios::binary | std::ios::in | std::ios::out);
    int n;
    std::cout << "1 to signup or 2 to login: ";
    std::cin >> n;
    bool lb = true;
    if (n == 1) {
        std::cout << "Enter username: ";
        std::cin >> us[0].ID;

        file.seekg(0, std::ios::end);
        int sizeF = file.tellg();
        sizeF = sizeF / 40;
        std::cout << "Size of file: " << sizeF << std::endl;

        us[0].PASS[0] = sizeF + '1';
        char winss = 48;
        char losses = 48;
        char tgame = 48;
        char time = 48;
        us[0].rec.wins = winss;
        us[0].rec.looses = losses;
        us[0].rec.times = time;
        us[0].rec.tgames = tgame;
        file.write((char*)&us[0].PASS, maxNameLength);
        file.write(us[0].ID, maxNameLength);
        file.write((char*)&us[0].rec.wins, 4);
        file.write((char*)&us[0].rec.looses, 4);
        file.write((char*)&us[0].rec.times, 4);
        file.write((char*)&us[0].rec.tgames, 4);
        // login(mine);
       // std::cout << "now press 2 to Login: ";
        // main();
    }
    else {
        char ID[maxNameLength] = {};
        char pass[maxNameLength] = {};
        std::cout << "id : ";
        std::cin >> us[0].ID;
        //std::cout << std::endl;
        std::cout << "username: ";
        std::cin >> us[0].PASS;
        
        for (int i = 1; i <= sizeof(file) / 40; i++) {
            bool tt = false;
            file.read(us[i].ID, 20);
            file.read((char*)&us[i].PASS, 36);
            if (us[0].ID[0] == us[i].ID[0]) {
                for (int j = 0; j < 20; j++) {
                    bool match = true;
                    if (us[0].PASS[j] != us[i].PASS[j]) {
                        match = false;
                        tt = false;
                        break;
                    }
                    if (match) {
                        tt = true;
                    }
                }
                if (tt) {
                    char winss = 48;
                    char losses = 48;
                    char tgame = 48;
                    char time = 0;
                    // cout << "MEOW";
                    game(mine, winss, losses, tgame, time);

                    file.seekg(0, std::ios::beg);
                    //  file.read((char*)&usd2[0].rec.wins, 20);
                   //   file.seekp(file.tellg() sizeof(user));
                    char aa;
                    for (int k = 1; k < 40; k++) {
                        file.read(usd[k].ID, 20);
                        file.read((char*)&usd[k].PASS, maxNameLength);
                        file.read((char*)&usd[k].rec.wins, 4);
                        file.read((char*)&usd[k].rec.looses, 4);
                        file.read((char*)&usd[k].rec.tgames, 4);
                        file.read((char*)&usd[k].rec.times, 4);
                        // file.read(aa, 4);
                    }
                    file.seekg(0, std::ios::beg);
                    int sizeF = file.tellg();
                    sizeF = sizeF;
                    for (int k = 1; k < (sizeof(file) / 56) + 1; k++) {
                        ff.write(usd[k].ID, maxNameLength);
                        ff.write((char*)&usd[k].PASS, maxNameLength);
                        if (k == i && winss > 48) {

                            usd[i].rec.wins++;
                            ff.write((char*)&usd[i].rec.wins, 4);
                            usd[i].rec.wins--;
                        }
                        else if (k == i && winss <= 48) {
                            ff.write((char*)&usd[i].rec.wins, 4);
                        }
                        if (k == i && losses > 48) {
                            usd[i].rec.looses++;
                            ff.write((char*)&usd[i].rec.looses, 4);
                            usd[i].rec.looses--;
                            //usd[i].rec.wins = winss;
                            //usd[i].rec.looses = losses;
                        }
                        else if (k == i && losses <= 48) {
                            ff.write((char*)&usd[i].rec.looses, 4);

                        }
                        if (k == i && tgame > 48) {
                            usd[i].rec.tgames++;
                            ff.write((char*)&usd[i].rec.tgames, 4);
                            //  ff.write((char*)&usd[i].rec.times, 4);
                            usd[i].rec.tgames--;
                        }
                        else if (k == i && tgame <= 48) {
                            ff.write((char*)&usd[i].rec.tgames, 4);
                        }
                        int ss = char(usd[k].rec.times);
                        if (k == i && char(time) + 48 > char(usd[k].rec.times)) {
                            usd[i].rec.times = time;
                            ff.write((char*)&usd[i].rec.times, 4);;
                        }
                        else if (k == i && char(time) <= char(usd[k].rec.times)) {
                            ff.write((char*)&usd[i].rec.times, 4);
                        }
                        if (k != i) {
                            ff.write((char*)&usd[k].rec.wins, 4);
                            ff.write((char*)&usd[k].rec.looses, 4);
                            ff.write((char*)&usd[k].rec.tgames, 4);
                            ff.write((char*)&usd[k].rec.times, 4);
                        }
                    }
                    std::cout << std::endl;
                    for (int k = 1; k < 20; k++) {
                        if (i == k) {
                            std::cout << "ID: " << int(usd[k].ID[0]) << std::endl;
                            std::cout << "NAME : " << usd[k].PASS << std::endl;
                            std::cout << "wins: " << char(usd[k].rec.wins) << std::endl;
                            std::cout << "lost : " << char(usd[k].rec.looses) << std::endl;
                            std::cout << "tgame : " << char(usd[k].rec.tgames) << std::endl;
                            std::cout << "time : " << int(usd[k].rec.times) << std::endl;
                            lb = false;
                        }
                    }

                    break;
                }


            }
        }
    }
    if (!lb) {
        std::fstream fr("creds.bin", std::ios::binary | std::ios::in);
        std::cout << "LEADERSHIP BOARD";
        std::cout << std::endl;
        for (int k = 1; k < 20; k++) {
            fr.read(usd[k].ID, 20);
            fr.read((char*)&usd[k].PASS, maxNameLength);
            fr.read((char*)&usd[k].rec.wins, 4);
            fr.read((char*)&usd[k].rec.looses, 4);
            fr.read((char*)&usd[k].rec.tgames, 4);
            fr.read((char*)&usd[k].rec.times, 4);
            // file.read(aa, 4);
        }
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                if (usd[j].rec.times > usd[j].rec.times)
                    std::swap(usd[j].rec.times, usd[j + 1].rec.times);
            }
        }

        for (int k = 1; k < 20; k++) {
            if (usd[k].ID[0] >= 48 && usd[k].ID[0] <= 60) {
                std::cout << "ID: " << usd[k].ID[0] << " NAME : " << usd[k].PASS << " wins: " << char(usd[k].rec.wins) << " lost : " << char(usd[k].rec.looses) << " tgame : " << char(usd[k].rec.tgames) << " time : " << int(usd[k].rec.times) << std::endl;
            }
        }
    }
    return 0;
}
