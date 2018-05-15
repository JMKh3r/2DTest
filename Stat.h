#pragma once

#include "stdafx.h"

#define STAT_DATA_LEN_P_2   5    // 32 элемента
//#define STAT_DATA_LEN_P_2   4    // 16 элементов
//#define STAT_DATA_LEN_P_2   3    // 8 элементов




#define STAT_DATA_LEN        ( (unsigned)1 << STAT_DATA_LEN_P_2 )
#define STAT_DATA_LEN_MASK  ( ~( ~(unsigned)0 << STAT_DATA_LEN_P_2) )



class CStat
{
private:
    unsigned m_First;
    unsigned m_Count;
    double m_Data[STAT_DATA_LEN];

public:

    void Clear()
    {
        memset(m_Data, 0, sizeof(m_Data));
        m_First = 0;
        m_Count = 0;
    }

    CStat()
    {
        Clear();
    }

    void Add(const double NewValue)
    {
        m_Data[(m_First + m_Count++) & STAT_DATA_LEN_MASK] = NewValue;
        if (m_Count <= STAT_DATA_LEN) return;
        m_Count = STAT_DATA_LEN;
        m_First = (m_First + 1) & STAT_DATA_LEN_MASK;
    }

    double Avg()
    {
        if (!m_Count) return 0;
        double Sum = 0;
        for (unsigned i = 0; i < m_Count; i++)
            Sum += m_Data[(m_First + i) & STAT_DATA_LEN_MASK];
        return Sum/m_Count;
    }


    //void Print()
    //    {
    //        std::cout << std::endl;
    //        std::cout << "First: " << m_First << std::endl;
    //        std::cout << "Count: " << m_Count << std::endl;
    //        std::cout << "Avg:   " << Avg() << std::endl;
    //        unsigned idx;
    //        for (unsigned i = 0; i < m_Count; i++)
    //        {
    //            idx = (m_First + i) & STAT_DATA_LEN_MASK;
    //            std::cout << "  (" << idx << "): " << m_Data[idx];
    //        }
    //        std::cout << std::endl;
    //    }

};



