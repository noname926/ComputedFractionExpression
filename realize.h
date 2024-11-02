#pragma once

#include <iostream>

using namespace std;

/* my_percent�����ռ����ṩ���ⲿ���õĽӿ� */
namespace my_percent
{
	class CPercent
	{
	public:
		CPercent()
		{
			sign = postive;
			sub = 0;
			deno = 1;
			percentDigit = 1;
		}
		CPercent(int sign, unsigned int sub, unsigned int deno)
		{
			MakeFromValue(sign, sub, deno);
			percentDigit = 1;
		}
		/* ������󷽷� */
		CPercent& MakeFromStr(const char* str);	//���ַ����еõ�����
		CPercent& MakeFromValue(int sign, unsigned int sub, unsigned int deno);	//���÷�����ֵ

		/* ������󷽷� */
		string GetString();	//���ط������ַ�����ʽ

		CPercent& operator+=(CPercent add);
	public:
		enum sign_type {
			minus,	//�����Ǹ���
			postive		//����������
		}sign;	/* �����ķ��� */
		unsigned int sub; //�����ķ���
		unsigned int deno; //�����ķ�ĸ(denominator)
		unsigned int percentDigit;  //�����ַ�����ʽ��λ��
	};

	int InspectExpression(const char* str, int str_len);		//���������ʽ�Ƿ���Ϲ淶 ����ֵ:0-����;-1�����Ϲ淶
	CPercent PercentExpressionCount(const char* str_expression, unsigned int str_len);	//����������ʽ��ֵ
}