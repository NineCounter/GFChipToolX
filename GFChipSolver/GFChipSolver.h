#pragma once
#include "solverStdafx.h"
#include <vector>
#include "GFChip.h"
#include "GFChipPlans.h"

struct SOLVER_EXP Solution
{
    //ƴ��ʹ�õ�оƬ��ţ�vector�е���ţ�����оƬ��ţ�
    int chipIndex[8];
    int chipNumber;     // ʹ�õ�оƬ����
    int blockAcu;       // ���ȸ���
    int blockFil;       // װ�����
    int blockDmg;       // �˺�����
    int blockDbk;       // �Ʒ�����
    int valueAcu;       // ������ֵ
    int valueFil;       // װ����ֵ
    int valueDmg;       // �˺���ֵ
    int valueDbk;       // �Ʒ���ֵ
    int planNumber;     // �ⷨ���

    Solution()
    {
        memset(chipIndex, -1, sizeof chipIndex);
        chipNumber = 0;
        blockAcu = 0;
        blockFil = 0;
        blockDmg = 0;
        blockDbk = 0;
        valueAcu = 0;
        valueFil = 0;
        valueDmg = 0;
        valueDbk = 0;
        planNumber = 0;
    }
};

//���Ը���
struct SOLVER_EXP Block
{
    double blockDmg, blockDbk, blockAcu, blockFil;
    Block(double dmg = 0.0,double dbk = 0.0,double acu = 0.0,double fil = 0.0):
        blockDmg(dmg), blockDbk(dbk), blockAcu(acu), blockFil(fil){}
};

//����Ԥ���plansѰ�����н�
SOLVER_EXP std::vector<Solution> solveChip(const std::vector<GFChip>& chips,const Plans& plans);
//Ѱ����target�����������maxOverflow�����н�
SOLVER_EXP std::vector<Solution> solveChip(const std::vector<GFChip>& chips,const Plans& plans, const Block& target, double maxOverflow = 2.0);
//Ѱ����target��4�����������error�����н�
SOLVER_EXP std::vector<Solution> solveChip(const std::vector<GFChip>& chips,const Plans& plans, const Block& target, const Block& error);
//������ɸѡ�������±�
SOLVER_EXP std::vector<int> findSolution(const std::vector<Solution>& solutions, int blockDmg, int blockDbk, int blockAcu, int blockFil);