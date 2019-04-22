#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include "split.h"
#include "GFChip.h"
#include "stdlib.h"
using namespace std;

char buffer[102400];
// chip save code rules by GFTool
const string rules[2] = { "InfinityFrost", "FatalChapters" };

// helper functions
char helperMain();
void helperExcel2Web();
void helperWeb2Excel();
void excel2Web(const char* filename, int color);

int main()
{
    start:
    if(auto t = helperMain(); t == '1')
    {
        helperExcel2Web();
    }
    else if(t == '2')
    {
        helperWeb2Excel();
    }
    else if(t == '0')
    {
        return 0;
    }
    else
    {
        // input error, restart.
        system("cls");
        goto start;
    }
    system("pause");
}

char helperMain()
{
    cout << "��ӭʹ�á���Ůǰ�� оƬ����ת������" << endl;
    cout << "���������ʼ���zaxs0130@gmail.com" << endl;
    cout << "��https://github.com/xxzl0130/GFChipConverter" << endl;
    cout << "===========================================================" << endl;
    cout << "�밴��ѡ��Ҫʹ�õĹ��ܣ�" << endl;
    cout << "1��Excel�����ߣ��� ====> ��ҳ�����£���" << endl;
    cout << "2����ҳ�����£���  ====> Excel�����ߣ���" << endl;
    cout << "0���˳�" << endl;
    return getchar();
}

void helperExcel2Web()
{
    system("cls");
    gets_s(buffer, sizeof(buffer));//clear input
    cout << "���ܣ�Excel�����ߣ��� ====> ��ҳ�����£���" << endl;
    cout << "����Excel��������ɫ�����Ȱ���ѡ��оƬ��ɫ��rΪ��ɫ������Ϊ��ɫ��" << endl;
    char color = getchar(); gets_s(buffer, sizeof(buffer));//clear input
    cout << "��ѡ�����ɫ�ǣ�" << (color == 'r' ? "��ɫ" : "��ɫ") << endl;
    cout << "�뽫excel��񱣴�Ϊcsv��ʽ������ʱѡ��оƬ�ܱ�������" << endl;
    cout << "Ȼ���ڴ˴�����csv�ļ�����ֱ�ӽ��ļ������˴���" << endl;
    cout << "�ļ�����";
    cin.getline(buffer, sizeof(buffer));
    excel2Web(buffer,color == 'r' ? 2 : 1);
    cout << "�����ѱ��浽SaveCode.txt�У���鿴" << endl;
}

void helperWeb2Excel()
{
    fstream fout;
    fout.open("SaveCode.csv", ios::out);
    std::vector<GFChip> chips;

    start2:
    system("cls");
    gets_s(buffer, sizeof(buffer));//clear input
    cout << "���ܣ���ҳ�����£���  ====> Excel�����ߣ���" << endl;
    cout << "��ֱ�ӽ��������ճ���ڴ˴������س���ȷ�ϡ�" << endl;
    
    gets_s(buffer, sizeof(buffer));
    if(strlen(buffer) < 10)
    {
        //error, restart
        goto start2;
    }
    auto list = split(string(buffer + 3, strlen(buffer) - 3 - 4),//skip begin and end
            '&');
    for(const auto& it : list)
    {
        // create chips
        chips.push_back(GFChip::createFromSaveCode(it));
    }

    for(const auto& it : chips)
    {
        fout << it.toExcelLine() << endl;
    }
    fout.close();

    cout << "�����ѱ��浽SaveCode.csv�У���򿪲鿴��" << endl;
    cout << "�����븴�Ƶ�Excel����е���Ӧλ�ã�����������." << endl;
}

void excel2Web(const char* filename,int color)
{
    fstream fin;
    fstream fout;
    fin.open(filename, ios::in);
    fout.open("SaveCode.txt", ios::out);
    std::vector<GFChip> chips;

    // skip useless line
    for(auto i = 0;i < 11;++i)
    {
        fin.getline(buffer, sizeof(buffer));
    }

    //read in
    while(true)
    {
        fin.getline(buffer, sizeof(buffer));
        if(strlen(buffer) < 10 || !isdigit(buffer[0]))
        {
            break;
        }
        auto chip = GFChip::createFromExcelLine(buffer);
        chip.chipColor = color;
        chips.push_back(chip);
    }

    auto chipNum = (*chips.rbegin()).chipNum;
    fout << "[" << rules[0][chipNum - 13 * int(chipNum / 13)] << "!";
    for(auto& it : chips)
    {
        fout << it.toSaveCode() << "&";
    }
    fout << "?" << rules[1][chipNum - 13 * int(chipNum / 13)] << "]";
    fin.close();
    fout.close();
}
