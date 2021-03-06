// Copyright (C) 2004 WoW Daemon
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#ifndef __TAXIMGR_H
#define __TAXIMGR_H

#define TAXI_TRAVEL_SPEED 32

class Player;

struct TaxiNode {
    uint32 id;
    float x,y,z;
    uint32 mapid;
    uint32 horde_mount;
    uint32 alliance_mount;
};

struct TaxiPathNode 
{
    float x,y,z;
    uint32 mapid;
};

class TaxiPath {
    friend class TaxiMgr;

public:
    TaxiPath() 
    { 
        m_length = 0;
    }

    ~TaxiPath() 
    {
        while(m_pathNodes.size())
        {
            TaxiPathNode *pn = m_pathNodes.begin()->second;
            m_pathNodes.erase(m_pathNodes.begin());
            delete pn;
        }
    }

    inline const float & getLength( ) const { return m_length; };
    void ComputeLen();
    void SetPosForTime(float &x, float &y, float &z, uint32 time, uint32* lastnode);
    inline uint32 GetID() { return id; }
    void SendMoveForTime(Player *riding, Player *to, uint32 time);
    void AddPathNode(uint32 index, TaxiPathNode* pn) { m_pathNodes[index] = pn; }
    inline uint32 GetNodeCount() { return m_pathNodes.size(); }
    TaxiPathNode* GetPathNode(uint32 i);
    inline uint32 GetPrice() { return price; }

protected:

    std::map<uint32, TaxiPathNode*> m_pathNodes;

    float m_length;
    uint32 id, to, from, price;
};


class TaxiMgr :  public Singleton < TaxiMgr >
{
public:
    TaxiMgr() 
    {
        _LoadTaxiNodes();
        _LoadTaxiPaths();
    }

    ~TaxiMgr() 
    { 
        while(m_taxiPaths.size())
        {
            TaxiPath *p = m_taxiPaths.begin()->second;
            m_taxiPaths.erase(m_taxiPaths.begin());
            delete p;
        }
        while(m_taxiNodes.size())
        {
            TaxiNode *n = m_taxiNodes.begin()->second;
            m_taxiNodes.erase(m_taxiNodes.begin());
            delete n;
        }
    }

    TaxiPath *GetTaxiPath(uint32 path);
    TaxiPath *GetTaxiPath(uint32 from, uint32 to);
    TaxiNode *GetTaxiNode(uint32 node);

    uint32 GetNearestTaxiNode( float x, float y, float z, uint32 mapid );
    bool GetGlobalTaxiNodeMask( uint32 curloc, uint32 *Mask );


private:
    void _LoadTaxiNodes();
    void _LoadTaxiPaths();

    HM_NAMESPACE::hash_map<uint32, TaxiNode*> m_taxiNodes;
    HM_NAMESPACE::hash_map<uint32, TaxiPath*> m_taxiPaths;
};

#define sTaxiMgr TaxiMgr::getSingleton()

#endif
