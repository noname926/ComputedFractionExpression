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

static inline unsigned int getIntdigit(int i)	//��ȡ���͵�λ��
{
	unsigned int ret_digit = 0;

	while (i)
	{
		ret_digit++;
		i /= 10;
	}

	return ret_digit;
}

static int getSignFromStr(const char* str, int str_len)		//���ַ�����ȡ���ţ�ֱ��ȡ������Ϊֹ,����ֵ��1Ϊ���ţ�0Ϊ���ţ�-1Ϊδȡ������
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
	CPercent& CPercent::MakeFromStr(const char* str)	//���ַ����еõ��������Զ�Ĭ��ȡ����
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

		this->sub = atoi(str);					//ȡ����
		int sub_digit = getIntdigit(this->sub);
		str += sub_digit + 1;		//������ӵ�λ������ת����ĸ��ͷ��λ��
		this->deno = atoi(str);					//ȡ��ĸ
		int deno_digit = getIntdigit(this->deno);

		this->percentDigit += sub_digit + deno_digit + 1;

		return (*this);
	}

	static unsigned int GetLCM(int a, int b)	//��ȡ��������С������
	{
		int i = 1;
		while ((a * i) % b)//����ʱ�����Ϊ0���٣���������ѭ��
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

	CPercent& CPercent::operator+=(		//ʵ�������������
		CPercent add)					//����Ϊ�β�ʹ�����ã���Ϊ�ڴ����غ��������ǻ�ı�add��ֵ�����ԡ�����
	{
		int least_public_deno = GetLCM(this->deno, add.deno);	//��ȡ��С����ĸ

		//��ͨ��
		this->sub *= (least_public_deno / this->deno);
		this->deno = least_public_deno;

		add.sub *= (least_public_deno / add.deno);
		add.deno = least_public_deno;
		//����
		if (this->sign == add.sign) //����������ǾͰѾ���ֵ���
		{
			//���
			this->sub += add.sub;
		}
		else if (this->sign == CPercent::minus)	//����һ��һ��
		{
			if (this->sub > add.sub)	//���磺-2+��+1����������ֵ���
			{
				this->sub = this->sub - add.sub;
			}
			else //this->sub <= add.sub		���磺-1+��+2��=0
			{
				//���ǽ�����ȡ��
				this->sign = CPercent::postive;
				//�ٽ�����ֵ�����
				this->sub = add.sub - this->sub;
			}
		}
		else	//����һ��һ��
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

	static inline int GetExpressionPercentN(const char* str, int str_len)	//�õ����ʽ�з����ĸ���
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

	// CFILTER_MAXCOUNT ������ CFilter ���а������ַ�������
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

		// ���ð�������list�ĸ�ʽ����: "abcd"��������Ϊa.b.c.d�ĸ��ַ�
		// list��һ����'\0'��β���ַ���
		// ע�⣺��������Ϊ CFILTER_MAX_WHITECOUNT ���ַ�
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
		// �ڰ����������һ���ַ�
		// ����ֵ���ҵ�������white_list�е�������δ�ҵ�����-1
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

	// ��������λ���м��ж��ٸ�'/'
	// ����:[start, end)Ҳ���Ǵ��ڵ���start��С��end
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
	* ��������б���м���ŵ�����
	* ����ֵ�����ŵ�����
	* ������str   --	�ַ�����ָ��
	*		start --	Ҫ�������ʼλ�ã������str��ƫ�ƣ�
	*		end	  --	��ֹλ�ã���������
	* ���䣺Ҫ�������� [start, end)
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

	// �������������ʽ�Ƿ���Ϲ���,���Ϸ���0�����򷵻�-1
	// ����1.б��������������֣�2.����ǰ�����������֣�3.��������֮��ֻ��һ��б�ˣ�4.������������������5.����б��֮��Ҫ����һ������
	int InspectExpression(const char* str, int str_len)
	{
		static CFilter filter_total("0123456789-+/");
		static CFilter filter_number("0123456789");
		static CFilter filter_sign("-+");
		int front_sing_index = 0; //ǰһ�����ŵ�λ��
		int front_diagonal_index = 0;

		for (int i = 0; i < str_len; i++)
		{
			char cur_c = str[i];

			if (filter_total.FindInWhiteList(cur_c) == -1)
			{
				// ���ｫ�ᱣ֤�ַ�����ȫ��Ϊ�Ϸ��ַ�
				return -1;
			}

			if (cur_c == '/')
			{
				// ���ｫ�ᱣ֤б�����߶�Ϊ����
				if (i == 0)
				{
					return -1;
				}

				if (!(IsCharNumber(str[i - 1]) && IsCharNumber(str[i + 1])))
				{
					// ǰ����һ����������
					return -1;
				}
				/* ����ᱣ֤����б��֮����ŵ����� */
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
				//����һ�δ��뱣֤�������ż�б�˵�����
				int diagonale_count = CountDiagonal(str, front_sing_index, i);	//�����ȡ�����ڵ�б������

				if (diagonale_count != 1)	//ֻ������һ��б��
				{
					return -1;
				}
				front_sing_index = i;
				// ��һ�δ��뱣֤���ŵ�ǰ�����������
				if (!(IsCharNumber(str[i - 1]) && IsCharNumber(str[i + 1])))
				{
					// ǰ����һ����������
					return -1;
				}
			}
		}

		return 0;
	}

	// ����������ʽ��ֵ
	// ����ֵ��CPercent --- �������ͣ���ʾ�����������ʽ��ֵ
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