#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <NitaDataTypes.h>

class Node
{
protected:
    std::string m_strName;
    std::string m_strPath;
    Node* m_nChild;
    Node* m_nSibling;

public:
    Node();
    Node(Node* parent, std::string name, std::string path);
    ~Node();
    Node* rec(Node* child);
    Node* getSubNode(LPCNStr str);
    Node* recName(Node* child, LPCNStr str);
    NVoid getSubNodeCount(NDword& dwCount);
    NVoid recCount(Node* child, NDword& dwCount);
    Node* getSubNodeByIdx(NDword dwIdx);
    LPCNStr getName();
    LPCNStr getPath();
    NBool deleteSubNode(LPCNStr str);
    NBool deleteAllSubNode();

};

Node::Node()
{
    this->m_strName = "";
    this->m_strPath = "";
    this->m_nChild = NULL;
    this->m_nSibling = NULL;
}

Node::Node(Node* parent, std::string strName, std::string strPath)
{
    this->m_strName = strName;
    this->m_strPath = strPath;
    this->m_nChild = NULL;
    this->m_nSibling = NULL;
    if(parent->m_nChild == NULL)
    {
        parent->m_nChild = this;
    }
    else
    {
        rec(parent->m_nChild)->m_nSibling = this;
    }
}

Node::~Node()
{
    delete this->m_nChild;
    delete this->m_nSibling;
}

Node* Node::rec(Node* node)
{
    if(node->m_nSibling == NULL)
    {
        return node;
    }
    else
    {
        return rec(node->m_nSibling);
    }
}

Node* Node::getSubNode(LPCNStr str)
{
    if(this->m_nChild != NULL)
    {
        return recName(this->m_nChild, str);
    }
    else
    {
        return NULL;
    }
}

Node* Node::recName(Node* node, LPCNStr str)
{
    if(node->m_strName == str)
    {
        return node;
    }
    else
    {
        if(node->m_nSibling != NULL){
            return recName(node->m_nSibling, str);
        }
        else
        {
            return NULL;
        }
    }
}

NVoid Node::getSubNodeCount(NDword& dwCount)
{
    if(this->m_nChild != NULL)
    {
        return recCount(this->m_nChild, dwCount);
    }
}

NVoid Node::recCount(Node* node, NDword& dwCount)
{
    dwCount++;
    if(node->m_nSibling != NULL)
    {
        recCount(node->m_nSibling, dwCount);
    }
}

Node* Node::getSubNodeByIdx(NDword dwIdx)
{
    Node* nodeRes = this->m_nChild;
    for(int i = 0; i < int(dwIdx); i++)
    {
        nodeRes = nodeRes->m_nSibling;
    }
    return nodeRes;
}

LPCNStr Node::getName()
{
    return m_strName.c_str();
}

LPCNStr Node::getPath()
{
    return m_strPath.c_str();
}

NBool Node::deleteSubNode(LPCNStr str)
{
    Node* nodeDel = this->getSubNode(str);
    if(nodeDel != NULL)
    {
        if(this->m_nChild == nodeDel)
        {
            this->m_nChild = nodeDel->m_nSibling;
            nodeDel->m_nSibling = NULL;
            delete nodeDel;
            return true;
        }
        else
        {
            Node* nSibling = nodeDel->m_nSibling;
            Node* nChild = this->m_nChild;
            while(nChild->m_nSibling != nodeDel)
            {
                nChild = nChild->m_nSibling;
            }
            nChild->m_nSibling = nSibling;
            delete nodeDel;
            return true;
        }
    }


    return false;
}

 NBool Node::deleteAllSubNode()
 {
    delete this->m_nChild;
    return true;
 }







































#endif // NODE_H
