#include "GFChipSolver.h"
#include <map>
#include <vector>
#include <algorithm>

//����ͬclass��оƬ����,keyΪclass��valueΪ��ź��Ƿ�ʹ��
typedef std::map<int, std::vector<std::pair<int,bool>>> ChipClassifiedMap;
static ChipClassifiedMap chipClassified;
static std::vector<Solution> solutions;
static Solution tmpSolution;
static const std::vector<GFChip>* chipsPtr = nullptr;

//����ͬclass��оƬ����
void classifyChip(const std::vector<GFChip>& chips);
//��鵱ǰоƬ�����Ƿ������ƴ�������Ҫ
bool satisfyPlan(const Plan& plan);
void findSolution(const Plan& plan, int planNumber, int k = 0);
//ͳ��ƴ��������
void calcSolution(Solution& s);

std::vector<Solution> solveChip(const std::vector<GFChip>& chips, const Plans& plans)
{
    solutions.clear();
    chipsPtr = &chips;
    classifyChip(chips);
    for(auto i = 0;i < plans.size();++i)
    {
        if(!satisfyPlan(plans[i]))
        {
            continue;
        }
        findSolution(plans[i], i);
    }

    return solutions;
}

std::vector<int> findSolution(const std::vector<Solution>& solutions, int blockDmg, int blockDbk, int blockAcu, int blockFil)
{
    std::vector<int> indexs;
    for(auto i = 0;i < solutions.size();++i)
    {
        const auto& it = solutions[i];
        if (it.blockDmg == blockDmg && it.blockAcu == blockAcu && it.blockDbk == blockDbk && it.blockFil == blockFil)
        {
            indexs.push_back(i);
        }
    }
    return indexs;
}

void classifyChip(const std::vector<GFChip>& chips)
{
    chipClassified.clear();
    for(auto i = 0;i < chips.size();++i)
    {
        const auto& chip = chips[i];
        if(!chipClassified.count(chip.chipType))
        {
            //����
            chipClassified[chip.chipType] = std::vector<std::pair<int, bool>>();
        }
        chipClassified[chip.chipType].emplace_back(i,false);
    }
}

bool satisfyPlan(const Plan& plan)
{
    bool ans = true;
    std::map<int, int> require;
    for(const auto& it : plan)
    {
        ++require[it];
    }

    for(const auto& it : require)
    {
        //ÿ��оƬ��������
        ans = ans && (chipClassified[it.first].size() >= it.second);
    }

    return ans;
}

void findSolution(const Plan& plan, int planNumber, int k)
{
    if(k >= plan.size())
    {
        calcSolution(tmpSolution);
        tmpSolution.chipNumber = k;
        tmpSolution.planNumber = planNumber;
        solutions.push_back(tmpSolution);
        return;
    }
    auto& chips = chipClassified[plan[k]];//��ȡ��ǰ�����ͺŵ�оƬ�б�
    for(auto& it : chips)
    {
        if(!it.second)
        {
            //оƬδ��ʹ��
            it.second = true;
            tmpSolution.chipIndex[k] = it.first;
            findSolution(plan, planNumber, k + 1);
            it.second = false;//�ָ���¼
        }
    }
}

void calcSolution(Solution& s)
{
    s.blockDbk = 0; s.valueDbk = 0;
    s.blockDmg = 0; s.valueDmg = 0;
    s.blockFil = 0; s.valueFil = 0;
    s.blockAcu = 0; s.valueAcu = 0;
    for(auto i = 0;i < s.chipNumber;++i)
    {
        const auto& chip = (*chipsPtr)[s.chipIndex[i]];
        auto value = chip.calcValue();
        s.blockDbk += chip.blockDbk; 
        s.valueDbk += value.blockDbk;
        s.blockDmg += chip.blockDmg; 
        s.valueDmg += value.blockDmg;
        s.blockFil += chip.blockFil; 
        s.valueFil += value.blockFil;
        s.blockAcu += chip.blockAcu; 
        s.valueAcu += value.blockAcu;
    }
}
