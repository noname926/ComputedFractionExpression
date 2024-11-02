# ComputedFractionExpression
计算分数表达式的值，支持加法和减法。  
realize.h中定义了my_percent命名空间，其中包含：  
  &emsp;&emsp;CPercent类，用来表示一个分数。  
  &emsp;&emsp;InspectExpression函数，用来检查一个分数表达式是否符合规则，返回值:0-正常;-1不符合规范。  
  &emsp;&emsp;PercentExpressionCount函数，用来计算一个分数表达式的结果，请确保分数表达式符合规范，返回值：CPercent，表示结果。  
# CPercent class
  &emsp;&emsp;CPercent()，默认构造函数，将符号设置为正号，分子设置为0，分母设置为1。    
  &emsp;&emsp;CPercent(int sign, unsigned int sub, unsigned int deno)，使用参数中的属性进行初始化。   
  &emsp;&emsp;CPercent& MakeFromStr(const char* str)，从一个字符串中获取到一个分数，用来初始化自己。    
  &emsp;&emsp;CPercent& MakeFromValue(int sign, unsigned int sub, unsigned int deno)，使用参数更改自己。    
  &emsp;&emsp;string GetString()，将自身转换为字符串形式，进行返回。    
  &emsp;&emsp;CPercent& operator+=(CPercent add)，重载+=符号，实现两个分数的相加（无论正负都可以），将结果保存到自身，并返回一个引用。    
  &emsp;&emsp;sign_type sign，这是一个枚举类型，用来表示正号或者符号，负号为minus，正号为postive。    
  &emsp;&emsp;unsigned int sub，表示分数的分子。    
  &emsp;&emsp;unsigned int deno，分数的分母(denominator)。    
  &emsp;&emsp;unsigned int percentDigit，分数字符串形式的位数。    
# InspectExpression函数
  &emsp;&emsp;int InspectExpression(const char* str, int str_len);    
  &emsp;&emsp;检查一个分数表达式是否符合规则。    
  &emsp;&emsp;参数：      
  &emsp;&emsp;&emsp;&emsp;        str    --    表达式字符串    
  &emsp;&emsp;&emsp;&emsp;        str_len  --  str的长度    
  &emsp;&emsp;返回值:    
  &emsp;&emsp;&emsp;&emsp;        0-正常;-1不符合规范    
# PercentExpressionCount函数
  &emsp;&emsp;CPercent PercentExpressionCount(const char* str_expression, unsigned int str_len);    
  &emsp;&emsp;计算一个分数表达式的值。    
  &emsp;&emsp;参数：    
  &emsp;&emsp;&emsp;&emsp;        str_expression  --    表达式    
  &emsp;&emsp;&emsp;&emsp;        str_len          --   表达式的长度    
  &emsp;&emsp;返回值：    
  &emsp;&emsp;&emsp;&emsp;        CPercent，表示结果，如果分母为0就说明计算出现了错误。    
# 最后
  &emsp;&emsp;提供了一个使用模版main.cpp，可以在VS运行    
