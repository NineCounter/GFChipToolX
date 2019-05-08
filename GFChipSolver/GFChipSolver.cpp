#include "GFChipSolver.h"
#include <map>
#include <vector>
#include <algorithm>

//����ͬclass��оƬ����,keyΪclass��valueΪ��ź��Ƿ�ʹ��
typedef std::map<int, std::vector<std::pair<int,bool>>> ChipClassifiedMap;
static ChipClassifiedMap chipClassified;
static std::vector<Solution> solutions;
static Solution tmpSolution;

//����ͬclass��оƬ����
void classifyChip(const std::vector<GFChip>& chips);
//��鵱ǰоƬ�����Ƿ������ƴ�������Ҫ
bool satisfyPlan(const Plan& plan);
void findSolution(const Plan& plan, int k = 0);
//ͳ��ƴ��������
void calcSolution(Solution& s);

std::vector<Solution> solveChip(const std::vector<GFChip>& chips, const Plans& plans)
{
    solutions.clear();
    classifyChip(chips);
    for(auto i = 0;i < plans.size();++i)
    {
        if(!satisfyPlan(plans[i]))
        {
            continue;
        }
        findSolution(plans[i]);
    }

    return solutions;
}

void classifyChip(const std::vector<GFChip>& chips)
{
    chipClassified.clear();
    for(auto i = 0;i < chips.size();++i)
    {
        const auto& chip = chips[i];
        if(!chipClassified.count(chip.chipClass))
        {
            //����
            chipClassified[chip.chipClass] = std::vector<std::pair<int, bool>>();
        }
        chipClassified[chip.chipClass].emplace_back(i,false);
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
        ans = ans && (chipClassified.count(it.first) >= it.second);
    }

    return ans;
}

void findSolution(const Plan& plan, int k)
{
    if(k >= plan.size())
    {
        calcSolution(tmpSolution);
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
            findSolution(plan, k + 1);
            it.second = false;//�ָ���¼
        }
    }
}

void calcSolution(Solution& s)
{
}
