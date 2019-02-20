/***************************************************
** @file: agreement.h
** @author: Mr.Chen
** @date: 2016/7/7
** @brief: Э�����
***************************************************/
#ifndef _AGREEMENT_H
#define _AGREEMENT_H

#include "type.h"
#include "thread.h"
#include "mcHash.h"

class Agreement
{
public:
	typedef mcHashMap<class HashStr, char*> HashArgv;
	void	parser(char* uri);

private:
	void	_loadArgv(HashArgv *hash, char* data);	// ��ȡUrl�ĸ�������,����hash
	int	  _type,  _flag;
	HashArgv _argv;
};

#endif // _AGREEMENT_H


