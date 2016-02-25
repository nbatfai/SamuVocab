#ifndef SamuBrain_H
#define SamuBrain_H

/**
 * @brief Samu has learnt the rules of Conway's Game of Life
 *
 * @file SamuBrain.h
 * @author  Norbert Bátfai <nbatfai@gmail.com>
 * @version 0.0.1
 *
 * @section LICENSE
 *
 * Copyright (C) 2015, 2016 Norbert Bátfai, batfai.norbert@inf.unideb.hu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * SamuBrain, exp. 4, cognitive mental organs: MPU (Mental Processing Unit), Q-- lerning, acquiring higher-order knowledge
 *
 * This is an example of the paper entitled "Samu in his prenatal development".
 *
 * Previous experiments
 *
 * Samu (Nahshon)
 * http://arxiv.org/abs/1511.02889
 * https://github.com/nbatfai/nahshon
 *
 * SamuLife
 * https://github.com/nbatfai/SamuLife
 * https://youtu.be/b60m__3I-UM
 *
 * SamuMovie
 * https://github.com/nbatfai/SamuMovie
 * https://youtu.be/XOPORbI1hz4
 *
 * SamuStroop
 * https://github.com/nbatfai/SamuStroop
 * https://youtu.be/6elIla_bIrw
 * https://youtu.be/VujHHeYuzIk
 */

#include <QThread>
#include <QDebug>
#include <sstream>
#include "SamuQl.h"
#include <vector>
#include <set>
#include <cstdlib>

class Habituation
{
    int mem {0};
    int err {0};

    static const int convergence_limit {
        //4000
        400
    };
    static const int error_limit {
        7
    };

    constexpr static int ma_limit {3};

    int msum[ma_limit];
    int asum[ma_limit];

    int masum {0};
    int mavsum {0};

    Habituation ( const Habituation & );
    Habituation & operator= ( const Habituation & );

public:

    Habituation() {
        clear();
    }

    bool is_habituation ( int , int , double & );
    bool is_newinput ( int sum, int vsum );
    void clear() {
        mem = 0;
        err = 0;

        for ( int ci {0}; ci<ma_limit; ++ci ) {
            msum[ci] = asum[ci] = 0;
        }

    }

};

typedef QL** MPU;

class MentalProcessingUnit
{
    int m_w {40}, m_h {30};
    MPU m_samuQl;
    Habituation m_habi;

    char **m_prev;
    char ** fr;
    char ** fp;

    MentalProcessingUnit ( const MentalProcessingUnit & );
    MentalProcessingUnit & operator= ( const MentalProcessingUnit & );


public:

    int sum, vsum;

    MentalProcessingUnit ( int w = 30, int h = 20 );
    ~MentalProcessingUnit();

    MPU getSamu() {
        return m_samuQl;
    }
    char ** getPrev() {
        return m_prev;
    }
    char ** getFp() {
        return fp;
    }
    char ** getFr() {
        return fr;
    }
    Habituation& getHabituation() {
        return m_habi;
    }

    void cls();

};

typedef MentalProcessingUnit* MORGAN;

class SamuBrain
{

    int m_w {40};
    int m_h {30};

    std::map<std::string, MORGAN> m_brain;
    MORGAN m_morgan;

    bool m_haveAlreadyLearnt {false};
    bool m_haveAlreadyLearntSignal {false};
    bool m_searching {false};
    long m_internal_clock {0};
    int m_haveAlreadyLearntTime {0};
    int m_maxLearningTime {0};
    int m_searchingStart {0};
    bool m_habituation {false};

    MORGAN newMPU ();
    int pred ( char **reality, char **predictions, int, int & );
    int pred ( MORGAN, char **reality, char **predictions, int, int & );
    void apred ( int r, int c, char **reality, char **predictions, int isLearning );
    void init_MPUs ( bool ex );
    std::string get_foobar ( MORGAN ) const;

    char *** fp;
    char *** fr;


    SamuBrain ( const SamuBrain & );
    SamuBrain & operator= ( const SamuBrain & );

    std::vector<int> prime = {
        2,
        3,
        5,
        7,
        11,
        13,
        17,
        19,
        23,
        29,
        31,
        37,
        41,
        43,
        47,
        53,
        59,
        61,
        67,
        71,
        73,
        79,
        83,
        89,
        97,
        101,
        103,
        107,
        109,
        113,
        127,
        131,
        137,
        139,
        149,
        151,
        157,
        163,
        167,
        173,
        179,
        181,
        191,
        193,
        197,
        199,
        211,
        223,
        227,
        229,
        233,
        239,
        241,
        251,
        257,
        263,
        269,
        271,
        277,
        281,
        283,
        293,
        307,
        311,
        313,
        317,
        331,
        337,
        347,
        349,
        353,
        359,
        367,
        373,
        379,
        383,
        389,
        397,
        401,
        409,
        419,
        421,
        431,
        433,
        439,
        443,
        449,
        457,
        461,
        463,
        467,
        479,
        487,
        491,
        499,
        503,
        509,
        521,
        523,
        541,
        547,
        557,
        563,
        569,
        571,
        577,
        587,
        593,
        599,
        601,
        607,
        613,
        617,
        619,
        631,
        641,
        643,
        647,
        653,
        659,
        661,
        673,
        677,
        683,
        691,
        701,
        709,
        719,
        727,
        733,
        739,
        743,
        751,
        757,
        761,
        769,
        773,
        787,
        797,
        809,
        811,
        821,
        823,
        827,
        829,
        839,
        853,
        857,
        859,
        863,
        877,
        881,
        883,
        887,
        907,
        911,
        919,
        929,
        937,
        941,
        947,
        953,
        967,
        971,
        977,
        983,
        991,
        997,
        1009,
        1013,
        1019,
        1021,
        1031,
        1033,
        1039,
        1049,
        1051,
        1061,
        1063,
        1069,
        1087,
        1091,
        1093,
        1097,
        1103,
        1109,
        1117,
        1123,
        1129,
        1151,
        1153,
        1163,
        1171,
        1181,
        1187,
        1193,
        1201,
        1213,
        1217,
        1223,
        1229,
        1231,
        1237,
        1249,
        1259,
        1277,
        1279,
        1283,
        1289,
        1291,
        1297,
        1301,
        1303,
        1307,
        1319,
        1321,
        1327,
        1361,
        1367,
        1373,
        1381,
        1399,
        1409,
        1423,
        1427,
        1429,
        1433,
        1439,
        1447,
        1451,
        1453,
        1459,
        1471,
        1481,
        1483,
        1487,
        1489,
        1493,
        1499,
        1511,
        1523,
        1531,
        1543,
        1549,
        1553,
        1559,
        1567,
        1571,
        1579,
        1583,
        1597,
        1601,
        1607,
        1609,
        1613,
        1619,
        1621,
        1627,
        1637,
        1657,
        1663,
        1667,
        1669,
        1693,
        1697,
        1699,
        1709,
        1721,
        1723,
        1733,
        1741,
        1747,
        1753,
        1759,
        1777,
        1783,
        1787,
        1789,
        1801,
        1811,
        1823,
        1831,
        1847,
        1861,
        1867,
        1871,
        1873,
        1877,
        1879,
        1889,
        1901,
        1907,
        1913,
        1931,
        1933,
        1949,
        1951,
        1973,
        1979,
        1987
    };

public:
    SamuBrain ( int w = 30, int h = 20 );
    ~SamuBrain();

    void learning ( char **reality, char **predictions, char *** fp, char *** fr );
    int getW() const;
    int getH() const;
    bool isSearching() const;
    int nofMPUs() const;
    std::string get_foobar() const;

    bool isHabituation() const {
        return m_habituation;
    }
    bool isLearned()  {
        // only a temporary trick
        bool ret = m_haveAlreadyLearntSignal;
        m_haveAlreadyLearntSignal = false;
        return ret;
    }

};

#endif
