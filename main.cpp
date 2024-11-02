#include "realize.h"
#include <string.h>
#include <stdio.h>

int main(int argc, const char* argv[])
{
	const char* str = "1/2+1/3+1/4";	//要计算的表达式
	my_percent::CPercent ret = my_percent::PercentExpressionCount(str, strlen(str));	//计算表达式的值(仅支持+和-)，内部对表达式的格式进行了检测

	if (ret.deno == 0)
	{
		printf("你输入的表达式格式不对!!!\n");
		return 0;
	}

	// 以下代码输出结果
	printf("%s", ret.GetString().c_str());

	return 0;
}