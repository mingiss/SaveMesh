/* --------------------------------------
 * MeshFile
 *      supporting .msh file format
 *
 */

#include "MeshFile.h"

int ElemSizes[MSH_MAX_NUM + 1] =
{
0,
2,  // MSH_LIN_2              1  
3,  // MSH_TRI_3              2  
4,  // MSH_QUA_4              3  
4,  // MSH_TET_4              4  
8,  // MSH_HEX_8              5  
6,  // MSH_PRI_6              6  
5,  // MSH_PYR_5              7  
3,  // MSH_LIN_3              8  
6,  // MSH_TRI_6              9  
9,  // MSH_QUA_9              10 
10, // MSH_TET_10             11 
27, // MSH_HEX_27             12 
18, // MSH_PRI_18             13 
14, // MSH_PYR_14             14 
1,  // MSH_PNT                15 
8,  // MSH_QUA_8              16 
20, // MSH_HEX_20             17 
15, // MSH_PRI_15             18 
13, // MSH_PYR_13             19 
9,  // MSH_TRI_9              20 
10, // MSH_TRI_10             21 
12, // MSH_TRI_12             22 
15, // MSH_TRI_15             23 
15, // MSH_TRI_15I            24 
21, // MSH_TRI_21             25 
4,  // MSH_LIN_4              26 
5,  // MSH_LIN_5              27 
6,  // MSH_LIN_6              28 
20, // MSH_TET_20             29 
35, // MSH_TET_35             30 
56, // MSH_TET_56             31 
22, // MSH_TET_22             32 
28, // MSH_TET_28             33 
0,  // MSH_POLYG_             34 
0,  // MSH_POLYH_             35 
16, // MSH_QUA_16             36 
25, // MSH_QUA_25             37 
36, // MSH_QUA_36             38 
12, // MSH_QUA_12             39 
16, // MSH_QUA_16I            40 
20, // MSH_QUA_20             41 
28, // MSH_TRI_28             42 
36, // MSH_TRI_36             43 
45, // MSH_TRI_45             44 
55, // MSH_TRI_55             45 
66, // MSH_TRI_66             46 
49, // MSH_QUA_49             47 
64, // MSH_QUA_64             48 
81, // MSH_QUA_81             49 
100, // MSH_QUA_100           50 
121, // MSH_QUA_121           51 
18, // MSH_TRI_18             52 
21, // MSH_TRI_21I            53 
24, // MSH_TRI_24             54 
27, // MSH_TRI_27             55 
30, // MSH_TRI_30             56 
24, // MSH_QUA_24             57 
28, // MSH_QUA_28             58 
32, // MSH_QUA_32             59 
36, // MSH_QUA_36I            60 
40, // MSH_QUA_40             61 
7,  // MSH_LIN_7              62 
8,  // MSH_LIN_8              63 
9,  // MSH_LIN_9              64 
10, // MSH_LIN_10             65 
11, // MSH_LIN_11             66 
2,  // MSH_LIN_B              67 
2,  // MSH_TRI_B              68 
0,  // MSH_POLYG_B            69 
2,  // MSH_LIN_C              70 
// TETS COMPLETE (6->10)
84, // MSH_TET_84             71 
120, // MSH_TET_120           72 
165, // MSH_TET_165           73 
220, // MSH_TET_220           74 
286, // MSH_TET_286           75
0,
0,
0,
// TETS INCOMPLETE (6->10)
34, // MSH_TET_34             79 
40, // MSH_TET_40             80 
46, // MSH_TET_46             81 
52, // MSH_TET_52             82 
58, // MSH_TET_58             83 
//                               
1,  // MSH_LIN_1              84 
1,  // MSH_TRI_1              85 
1,  // MSH_QUA_1              86 
1,  // MSH_TET_1              87 
1,  // MSH_HEX_1              88 
1,  // MSH_PRI_1              89 
40, // MSH_PRI_40             90 
75, // MSH_PRI_75             91 
// HEXES COMPLETE (3->9)
64, // MSH_HEX_64             92 
125, // MSH_HEX_125           93 
216, // MSH_HEX_216           94 
343, // MSH_HEX_343           95 
512, // MSH_HEX_512           96 
729, // MSH_HEX_729           97 
1000, // MSH_HEX_1000         98 
// HEXES INCOMPLETE (3->9)
32, // MSH_HEX_32             99 
44, // MSH_HEX_44             100
56, // MSH_HEX_56             101
68, // MSH_HEX_68             102
80, // MSH_HEX_80             103
92, // MSH_HEX_92             104
104, // MSH_HEX_104           105
// PRISMS COMPLETE (5->9)
126, // MSH_PRI_126           106
196, // MSH_PRI_196           107
288, // MSH_PRI_288           108
405, // MSH_PRI_405           109
550, // MSH_PRI_550           110
// PRISMS INCOMPLETE (3->9)
24, // MSH_PRI_24             111
33, // MSH_PRI_33             112
42, // MSH_PRI_42             113
51, // MSH_PRI_51             114
60, // MSH_PRI_60             115
69, // MSH_PRI_69             116
78, // MSH_PRI_78             117
// PYRAMIDS COMPLETE (3->9)
30, // MSH_PYR_30             118
55, // MSH_PYR_55             119
91, // MSH_PYR_91             120
140, // MSH_PYR_140           121
204, // MSH_PYR_204           122
285, // MSH_PYR_285           123
385, // MSH_PYR_385           124
// PYRAMIDS INCOMPLETE (3->9)
21, // MSH_PYR_21             125
29, // MSH_PYR_29             126
37, // MSH_PYR_37             127
45, // MSH_PYR_45             128
53, // MSH_PYR_53             129
61, // MSH_PYR_61             130
69, // MSH_PYR_69             131
// Additional types              
1,  // MSH_PYR_1              132
1,  // MSH_PNT_SUB            133
2,  // MSH_LIN_SUB            134
3,  // MSH_TRI_SUB            135
4,  // MSH_TET_SUB            136
16, // MSH_TET_16             137
3,  // MSH_TRI_MINI           138
4,  // MSH_TET_MINI           139
4,  // MSH_TRIH_4             140
};


bool MeshFile::init(SmLogger* plog)
{
    bool retc = true;

    m_plog = plog;
    if (!m_plog)
        retc = false;

    return retc;
}

bool MeshFile::open(const char *fname)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && ((!fname) || (!*fname)))
    {
        m_plog->msg(__func__, "No file name given!");
        retc = false;
    }

    if (retc)
    {
        m_sMeshFName = fname;

        m_fMeshFile.open(m_sMeshFName, ios::out);
        if (m_fMeshFile.bad())
        {
            string msg = "Error to open the file " + m_sMeshFName + "!";
            m_plog->msg(__func__, msg.c_str());
            retc = false;
        }
    }

    return retc;
}

bool MeshFile::close(void)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && (!m_fMeshFile.is_open()))
    {
        string msg = "File " + m_sMeshFName + " not open!";
        m_plog->msg(__func__, msg.c_str());
        retc = false;
    }

    if (retc)
    {
        m_fMeshFile << "$EndElements" << endl;

        m_fMeshFile.close();
        if (m_fMeshFile.bad())
        {
            string msg = "Error closing the file " + m_sMeshFName + "!";
            m_plog->msg(__func__, msg.c_str());
            retc = false;
        }
        else
        {
            string msg = "Mesh saved to the file " + m_sMeshFName + ".";
            m_plog->msg(__func__, msg.c_str());
        }
    }

    return retc;
}

bool MeshFile::writeNodeHeader(void)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && (!m_fMeshFile.is_open()))
    {
        string msg = "File " + m_sMeshFName + " not open!";
        m_plog->msg(__func__, msg.c_str());
        retc = false;
    }

    if (retc)
    {
        m_fMeshFile << "$MeshFormat" << endl;
        m_fMeshFile << MSH_VERS << " " << MshFormats::ascii << " " << sizeof(size_t) << endl;
        m_fMeshFile << "$EndMeshFormat" << endl;

        m_fMeshFile << "$Nodes" << endl;

        // entities header
        m_fMeshFile << m_iNumOfPointEntities // num of entities
            << " " << m_iNumOfPoints // num of nodes in all entities
            << " " << 1 << " " << m_iNumOfPoints // first and last tag
            << endl;
    }

    if (retc && m_fMeshFile.bad())
    {
        string msg = "Error writing to the file " + m_sMeshFName + "!";
        m_plog->msg(__func__, msg.c_str());
        retc = false;
    }

    return retc;
}

bool MeshFile::writeElemsHeader(void)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && (!m_fMeshFile.is_open()))
    {
        string msg = "File " + m_sMeshFName + " not open!";
        m_plog->msg(__func__, msg.c_str());
        retc = false;
    }

    if (retc)
    {
        m_fMeshFile << "$EndNodes" << endl;

        m_fMeshFile << "$Elements" << endl;

        // entities header
        m_fMeshFile << m_iNumOfElemEntities
            << " " << m_iNumOfElems // num of elements in all entities
            << " " << 1 << " " << m_iNumOfElems // first and last element tag
            << endl;
    }

    if (retc && m_fMeshFile.bad())
    {
        string msg = "Error writing to the file " + m_sMeshFName + "!";
        m_plog->msg(__func__, msg.c_str());
        retc = false;
    }

    return retc;
}


bool MeshFile::writePoints(const vector<Ptr<Point3D>>& points, int& entity_tag, int& point_tag)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && (!m_fMeshFile.is_open()))
    {
        string msg = "File " + m_sMeshFName + " not open!";
        m_plog->msg(__func__, msg.c_str());
        retc = false;
    }

    if (retc)
    {
        size_t num_of_points = points.size();

        // entity header
        m_fMeshFile << 2 // num of dimensions
            << " " << entity_tag++
            << " " << false // no parametric coordinates
            << " " << num_of_points // num of nodes in the entity
            << endl;
        // node tags
        for (size_t ii = 0; ii < num_of_points; ii++)
            m_fMeshFile << point_tag++ << endl;

        for (vector<Ptr<Point3D>>::const_iterator it = points.cbegin(); (it != points.cend()) && retc; it++)
        {
            vector<double> coords = (*it)->asArray();
            if (coords.size() == 0)
            {
                m_plog->msg(__func__, "Empty point coordinates!");
                retc = false;
            }
            if (retc)
            {
                for (vector<double>::iterator id = coords.begin(); (id != coords.end()) && retc; id++)
                    m_fMeshFile << *id << " ";
                m_fMeshFile << endl;
            }
        }
    }

    if (retc && m_fMeshFile.bad())
    {
        string msg = "Error writing to the file " + m_sMeshFName + "!";
        m_plog->msg(__func__, msg.c_str());
        retc = false;
    }

    return retc;
}

bool MeshFile::writeElems(const vector<ElemEntity>& elems, int& entity_tag, int& elem_tag, const /* int& */ size_t& first_point_tag)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && (!m_fMeshFile.is_open()))
    {
        string msg = "File " + m_sMeshFName + " not open!";
        m_plog->msg(__func__, msg.c_str());
        retc = false;
    }

    if (retc) for (vector<ElemEntity>::const_iterator ie = elems.cbegin(); (ie != elems.cend()) && retc; ie++)
    {
        int elem_type = ie->m_iElemType;
        if ((elem_type < 0) || (elem_type >= MSH_MAX_NUM))
        {
            stringstream out_str;
            out_str << "Invalid elem type: " << elem_type << "!";
            m_plog->msg(__func__, out_str.str().c_str());
            retc = false;
        }

        int elem_size = 1;
        if (retc)
        {
            elem_size = ElemSizes[elem_type];
            if (elem_size < 1)
            {
                stringstream out_str;
                out_str << "Invalid elem size: " << elem_size << "!";
                m_plog->msg(__func__, out_str.str().c_str());
                retc = false;
            }
        }

        size_t num_of_elems = 0;
        if (retc)
        {
            num_of_elems = ie->m_Elems.size() / elem_size;

            m_fMeshFile << 2 // num of dimensions
                << " " << entity_tag++
                << " " << elem_type
                << " " << num_of_elems << endl;

            int ix = 0;
            for (vector<int>::const_iterator it = ie->m_Elems.cbegin(); (it != ie->m_Elems.cend()) && retc; it++)
            {
                if (ix % elem_size == 0)
                    m_fMeshFile << elem_tag++;
                m_fMeshFile << " " << *it + first_point_tag;
                if (ix % elem_size == elem_size - 1)
                    m_fMeshFile << endl;
                ix++;
            }
        }
    }

    if (retc && m_fMeshFile.bad())
    {
        string msg = "Error writing to the file " + m_sMeshFName + "!";
        m_plog->msg(__func__, msg.c_str());
        retc = false;
    }

    return retc;
}
