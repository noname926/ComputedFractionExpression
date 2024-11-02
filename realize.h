#pragma once

#include <iostream>

using namespace std;

/* my_percent命名空间中提供给外部调用的接口 */
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
		/* 构造对象方法 */
		CPercent& MakeFromStr(const char* str);	//从字符串中得到分数
		CPercent& MakeFromValue(int sign, unsigned int sub, unsigned int deno);	//设置分数的值

		/* 输出对象方法 */
		string GetString();	//返回分数的字符串形式

		CPercent& operator+=(CPercent add);
	public:
		enum sign_type {
			minus,	//符号是负号
			postive		//符号是正号
		}sign;	/* 分数的符号 */
		unsigned int sub; //分数的分子
		unsigned int deno; //分数的分母(denominator)
		unsigned int percentDigit;  //分数字符串形式的位数
	};

	int InspectExpression(const char* str, int str_len);		//检查分数表达式是否符合规范 返回值:0-正常;-1不符合规范
	CPercent PercentExpressionCount(const char* str_expression, unsigned int str_len);	//计算分数表达式的值
}