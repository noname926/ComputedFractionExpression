#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include "realize.h"

using namespace std;

static inline bool IsCharNumber(char ch)
{
	if (ch >= '0' && ch <= '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}

static inline bool IsCharSign(char ch)
{
	if (ch == '+' || ch == '-')
	{
		return true;
	}
	else
	{
		return false;
	}
}

static inline unsigned int getIntdigit(int i)	//获取整型的位数
{
	unsigned int ret_digit = 0;

	while (i)
	{
		ret_digit++;
		i /= 10;
	}

	return ret_digit;
}

static int getSignFromStr(const char* str, int str_len)		//从字符串中取符号，直到取到符号为止,返回值：1为正号，0为符号，-1为未取到符号
{
	for (int i = 0; i < str_len; i++)
	{
		if (str[i] == '+')
		{
			return 1;
		}
		else if (str[i] == '-')
		{
			return 0;
		}
	}
	return -1;
}

namespace my_percent
{
	CPercent& CPercent::MakeFromStr(const char* str)	//从字符串中得到分数，自动默认取符号
	{
		int i;

		this->sign = (sign_type)1;
		this->percentDigit = 0;
		int str_len = strlen(str);

		for (i = 0; i < str_len; i++)
		{
			if (IsCharSign(*str))
			{
				this->sign = (sign_type)getSignFromStr(str, strlen(str));
				this->percentDigit = 1;
			}
			else if (IsCharNumber(*str))
			{
				break;
			}
			str++;
		}

		this->sub = atoi(str);					//取分子
		int sub_digit = getIntdigit(this->sub);
		str += sub_digit + 1;		//计算分子的位数，跳转到分母开头的位置
		this->deno = atoi(str);					//取分母
		int deno_digit = getIntdigit(this->deno);

		this->percentDigit += sub_digit + deno_digit + 1;

		return (*this);
	}

	static unsigned int GetLCM(int a, int b)	//获取两数的最小公倍数
	{
		int i = 1;
		while ((a * i) % b)//整除时，结果为0（假），不进入循环
		{
			i++;
		}

		return (a * i);
	}

	string CPercent::GetString()
	{
		string ret;

		if (this->sign == 1)
		{
			ret += '+';
		}
		else
		{
			ret += '-';
		}

		ret += std::to_string(this->sub);
		ret += '/';
		ret += std::to_string(this->deno);

		return ret;
	}

	CPercent& CPercent::operator+=(		//实现两个分数相加
		CPercent add)					//这里为何不使用引用，因为在此重载函数中我们会改变add的值，所以。。。
	{
		int least_public_deno = GetLCM(this->deno, add.deno);	//获取最小公分母

		//先通分
		this->sub *= (least_public_deno / this->deno);
		this->deno = least_public_deno;

		add.sub *= (least_public_deno / add.deno);
		add.deno = least_public_deno;
		//计算
		if (this->sign == add.sign) //符号相等我们就把绝对值相加
		{
			//相加
			this->sub += add.sub;
		}
		else if (this->sign == CPercent::minus)	//符号一负一正
		{
			if (this->sub > add.sub)	//例如：-2+（+1），将绝对值相减
			{
				this->sub = this->sub - add.sub;
			}
			else //this->sub <= add.sub		例如：-1+（+2）=0
			{
				//我们将符号取反
				this->sign = CPercent::postive;
				//再将绝对值相相减
				this->sub = add.sub - this->sub;
			}
		}
		else	//符号一正一负
		{
			if (this->sub > add.sub)
			{
				this->sub = this->sub - add.sub;
			}
			else //this->sub <= add.sub
			{
				this->sign = CPercent::minus;
				this->sub = add.sub - this->sub;
			}
		}

		return (*this);
	}

	CPercent& CPercent::MakeFromValue(int sign, unsigned int sub, unsigned int deno)
	{
		this->sign = (sign_type)sign;
		this->sub = sub;
		this->deno = deno;

		return (*this);
	}

	static inline int GetExpressionPercentN(const char* str, int str_len)	//得到表达式中分数的个数
	{
		int ret_count = 0;
		for (int i = 0; i < str_len; i++)
		{
			if (str[i] == '/')
			{
				ret_count++;
			}
		}
		return ret_count;
	}

	// CFILTER_MAXCOUNT 定义了 CFilter 类中白名单字符最大个数
#define CFILTER_MAX_WHITECOUNT 100
	class CFilter
	{
	public:
		CFilter()
		{
		}
		CFilter(const char* white_list)
		{
			SetWhiteList(white_list);
		}

		// 设置白名单，list的格式例如: "abcd"，白名单为a.b.c.d四个字符
		// list是一个以'\0'结尾的字符串
		// 注意：最大个数定为 CFILTER_MAX_WHITECOUNT 个字符
		void SetWhiteList(const char* list)
		{
			white_list_len = strlen(list);
			if (white_list_len > CFILTER_MAX_WHITECOUNT)
			{
				white_list_len = 0;
				return;
			}
			memcpy(white_list, list, white_list_len);
		}
		// 在白名单里查找一个字符
		// 返回值：找到返回在white_list中的索引，未找到返回-1
		int FindInWhiteList(char c)
		{
			for (int i = 0; i < white_list_len; i++)
			{
				if (white_list[i] == c)
				{
					return i;
				}
			}

			return -1;
		}
	private:
		char white_list[CFILTER_MAX_WHITECOUNT] = { 0 };
		int white_list_len = 0;
	};

	// 计算两个位置中间有多少个'/'
	// 区间:[start, end)也就是大于等于start，小于end
	int CountDiagonal(const char* str, int start, int end)
	{
		int ret = 0;

		for (int i = start; i < end; i++)
		{
			if (str[i] == '/')
			{
				++ret;
			}
		}

		return ret;
	}

	/*
	* 计算两个斜杆中间符号的数量
	* 返回值：符号的数量
	* 参数：str   --	字符串的指针
	*		start --	要计算的起始位置（相较于str的偏移）
	*		end	  --	终止位置（不包含）
	* 补充：要检查的区间 [start, end)
	*/
	int CountSign(const char* str, int start, int end)
	{
		int r = 0;

		for (int i = start; i < end; i++)
		{
			if (str[i] == '+' || str[i] == '-')
			{
				r++;
			}
		}

		return r;
	}

	// 检查分数算术表达式是否符合规则,符合返回0，否则返回-1
	// 规则：1.斜杆两侧必须是数字，2.符号前后必须得是数字，3.两个符号之间只有一个斜杆，4.不包含各种其他符号5.两个斜杆之间要得有一个符号
	int InspectExpression(const char* str, int str_len)
	{
		static CFilter filter_total("0123456789-+/");
		static CFilter filter_number("0123456789");
		static CFilter filter_sign("-+");
		int front_sing_index = 0; //前一个符号的位置
		int front_diagonal_index = 0;

		for (int i = 0; i < str_len; i++)
		{
			char cur_c = str[i];

			if (filter_total.FindInWhiteList(cur_c) == -1)
			{
				// 这里将会保证字符串中全部为合法字符
				return -1;
			}

			if (cur_c == '/')
			{
				// 这里将会保证斜杆两边都为数字
				if (i == 0)
				{
					return -1;
				}

				if (!(IsCharNumber(str[i - 1]) && IsCharNumber(str[i + 1])))
				{
					// 前后有一个不是数字
					return -1;
				}
				/* 这里会保证两个斜杆之间符号的数量 */
				if (front_diagonal_index != 0)
				{
					int signc = CountSign(str, front_diagonal_index, i);
					if (signc != 1)
					{
						return -1;
					}
				}
				front_diagonal_index = i;
			}
			else if ((cur_c == '-' || cur_c == '+') && i != 0)
			{
				//这里一段代码保证两个符号间斜杆的数量
				int diagonale_count = CountDiagonal(str, front_sing_index, i);	//这里获取区间内的斜杆数量

				if (diagonale_count != 1)	//只允许有一个斜杆
				{
					return -1;
				}
				front_sing_index = i;
				// 这一段代码保证符号的前后必须是数字
				if (!(IsCharNumber(str[i - 1]) && IsCharNumber(str[i + 1])))
				{
					// 前后有一个不是数字
					return -1;
				}
			}
		}

		return 0;
	}

	// 计算分数表达式的值
	// 返回值：CPercent --- 分数类型，表示整个分数表达式的值
	CPercent PercentExpressionCount(const char* str_expression, unsigned int str_len)
	{
		if (
			InspectExpression(str_expression, str_len)
			)
		{
			return CPercent(CPercent::minus, 0, 0);
		}


		CPercent ahead_percent;
		int PrecentCount = GetExpressionPercentN(str_expression, str_len);

		for (int i = 0; i < str_len && PrecentCount>0; i++)
		{
			if (IsCharSign(str_expression[i]) || IsCharNumber(str_expression[i]))
			{
				PrecentCount--;

				CPercent percent;

				percent.MakeFromStr(&str_expression[i]);
				if (i == 0)
				{
					ahead_percent = percent;
				}
				else
				{
					ahead_percent += percent;
				}

				i += percent.percentDigit - 1;
			}
		}

		return ahead_percent;
	}
}