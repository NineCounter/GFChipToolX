#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <conio.h>
#include <cstdlib>
#include <stdio.h>
#include <cctype>
#include <ctime>
#include <time.h>
#include <sys/stat.h>

#include "split.h"
#include "GFChip.h"
#include <ctype.h>
using namespace std;

char *buffer;
// chip save code rules by GFTool
const string rules[2] = { "InfinityFrost", "FatalChapters" };
string timeStr;

// helper functions
void helperMain();
void excel2Web(int color);
void web2Excel();
int fileSize(const char* filename);
//��ȡ�ļ����ж��ļ����ͣ�1Ϊ��ҳ���룬2ΪExcel��0Ϊ�����ļ�������ȫ��buffer��
int readFile(const char* filename);

int main(int argc,char** argv)
{
    //��ȡʱ��
    auto timer = time(NULL);
    struct tm timeS;
    localtime_s(&timeS,&timer);
    stringstream ss;
    ss << timeS.tm_year + 1900 << "-" << timeS.tm_mon << "-" << timeS.tm_mday << " " << timeS.tm_hour << "-" << timeS.tm_min << "-" << timeS.tm_sec;
    timeStr = ss.str();

    const char* filename;
    string tmpStr;
    int color = -1;
    if(argc >= 2)
    {
        filename = argv[1];
        if(argc >= 3)
        {
            color = argv[2][0] == 'r' ? 2 : 1;
        }
    }
    else
    {
        helperMain();
        cin >> tmpStr;
        getchar();
        filename = tmpStr.c_str();
    }
    switch (readFile(filename))
    {
    case 1:
        web2Excel();
        break;
    case 2:
        if(color <= 0)
        {
            //��ɫδ��ʼ��
            cout << "����Excel��������ɫ���밴��ѡ��оƬ��ɫ��rΪ��ɫ��bΪ��ɫ:";
            char c;
            while(true)
            {
                c = tolower(getchar());
                if (c == 'r' || c == 'b')
                    break;
                else
                    while (getchar() != '\n');
                cout << "����Excel��������ɫ���밴��ѡ��оƬ��ɫ��rΪ��ɫ��bΪ��ɫ:";
            }
            color = c == 'r' ? 2 : 1;
        }
        excel2Web(color);
        break;
    default:
        cout << "��������ȷ���ļ���" << endl;
        break;
    }
    cout << "ת����ϣ����ڳ���Ŀ¼��Ѱ����Ӧ�ļ���\nллʹ�á�" << endl;
    system("pause");
}

void helperMain()
{
    cout << "��ӭʹ�á���Ůǰ�� оƬ����ת������" << endl;
    cout << "���������ʼ���zaxs0130@gmail.com" << endl;
    cout << "��https://github.com/xxzl0130/GFChipConverter" << endl;
    cout << "========================================================\n" << endl;
    cout << "ʹ��˵����" << endl;
    cout << "�����У�GFChipConverter <filename> [color(r/b)]" << endl;
    cout << "Example��GFChipConverter ��װоƬ������-��.csv r" << endl;
    cout << "��Ӵ˴������ļ�����" << endl << endl;
    cout << "�뽫excel��񱣴�Ϊcsv��ʽ������ʱѡ��оƬ�ܱ�������" << endl;
    cout << "����ҳ��洢���뱣��ΪTxt�ļ���" << endl;
    cout << "������Ҫת�����ļ�������ֱ�ӽ��ļ�����˴��ڣ�Ȼ�󰴻س���" << endl << endl;
    cout << "�ļ�����";
}

void web2Excel()
{
    auto list = split(string(buffer + 2, strlen(buffer) - 3 - 2),//skip begin and end
        '&');

    std::vector<GFChip> redChips, blueChips;
    for (const auto& it : list)
    {
        // create chips
        auto chip = GFChip::createFromSaveCode(it);
        if (chip.chipType == 9 || chip.chipType == 10 || chip.chipType > 110 || chip.chipType == 81 || chip.chipType == 82)
        {//5-block-2-type not used in excel
            continue;
        }
        if (chip.chipColor == 2)
        {
            chip.chipNum = redChips.size() + 1;
            redChips.push_back(chip);
        }
        else
        {
            chip.chipNum = blueChips.size() + 1;
            blueChips.push_back(chip);
        }
    }

    fstream fout;
    fout.open("RedChips " + timeStr + ".csv", ios::out);
    fout << "оƬ���,��״����,ɱ��,�Ʒ�,����,װ��,ǿ���ȼ�,ɱ��,�Ʒ�,����,װ��," << endl;
    for (const auto& it : redChips)
    {
        fout << it.toExcelLine() << endl;
    }
    fout.close();
    fout.open("BlueChips " + timeStr + ".csv", ios::out);
    fout << "оƬ���,��״����,ɱ��,�Ʒ�,����,װ��,ǿ���ȼ�,ɱ��,�Ʒ�,����,װ��," << endl;
    for (const auto& it : blueChips)
    {
        fout << it.toExcelLine() << endl;
    }
    fout.close();
}

void excel2Web(int color)
{
    std::vector<GFChip> chips;
    //read in
    auto lines = split(buffer, '\n');
    lines.erase(lines.begin(), lines.begin() + 11);
    for(const auto& it : lines)
    {
        if(it.size() < 10 || !isdigit(it[0]))
        {
            break;
        }
        auto chip = GFChip::createFromExcelLine(it);
        chip.chipColor = color;
        chips.push_back(chip);
    }
    if(chips.empty())
    {
        return;
    }
    auto chipNum = (*chips.rbegin()).chipNum;
    fstream fout;
    fout.open("SaveCode" + timeStr + ".txt", ios::out);
    fout << "[" << rules[0][chipNum - 13 * int(chipNum / 13)] << "!";
    for(auto& it : chips)
    {
        fout << it.toSaveCode() << "&";
    }
    fout << "?" << rules[1][chipNum - 13 * int(chipNum / 13)] << "]";
    fout.close();
}

int fileSize(const char* filename)
{
    struct stat statbuf;
    if (stat(filename, &statbuf) != 0) 
        return 0;//��ȡʧ�ܡ�
    return statbuf.st_size;//�����ļ���С��
}

int readFile(const char* filename)
{
    auto size = fileSize(filename);
    if (size <= 10)//too short
    {
        return 0;
    }
    size += 10;
    buffer = new char[size];
    memset(buffer, 0, size);
    FILE *fin;
    fopen_s(&fin, filename, "r");
    if(fin == NULL)
    {
        return -1;
    }
    fread_s(buffer, size, sizeof(char), size, fin);
    fclose(fin);

    auto len = strlen(buffer);
    while(buffer[len - 1] == '\n' || buffer[len - 1] == '\r' || buffer[len - 1] == ' ' || buffer[len - 1] < 0)
    {
        //�ų�ĩβ�հ�
        buffer[len - 1] = '\0';
        --len;
    }
    auto t = &buffer[len - 10];
    if(buffer[0] == '[' && isalpha(buffer[1]) && buffer[2] == '!' && buffer[len - 1] == ']')
    {
        //��У����ҳ�����
        return 1;
    }
    else if(strstr(buffer,"0,1,5,13,8,6"))
    {
        //��excel���һ�ι̶��ı�ΪУ��
        return 2;
    }
    else
    {
        return 0;
    }
}
