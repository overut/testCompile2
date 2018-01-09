//flex架构:

struct kc_token
{
	int num; //token num defined by bison
	int sub_type;  
	int sub_tupe1;
	int start_char;
	int char_length;
	int line_no;
	int col_no;
	char* ps_id;  //存储当前字符串

	const char* get_string()
	{
		return ps_id;
	}
}
#define SET_TOKEN(nu,st1,st2,sp,ep) do{\
	token.num = nu;
	token.sub_type = st1;\
	token.sub_type2 = st2;\
	token.line_no = m_line_num;\
	token.start_char = (sp);\
	token.char_length = (ep) - (sp) +1;\
	token.col_no = (ep) - old_cur_ptr;\
} while(0)
	
//token typede
enum tokenType{
	
}
//m_script_text存储所有文本信息
while(m_script_text[cur_ptr != 0x0000])
{
	//逐行解析
	ret = gen_tokens_by_line(cur_ptr);//传递引用cur_ptr
	//判断是否有换行，若有
	m_line_num++//记录行数
	cur_ptr++
}

//逐行解析过程（暂不支持多行/* */注释）：
while(1)
{
	kc_token token;
	//token数据清空
	sz_char = m_script_text[cur_ptr];
	//判断是否为结束符 0x0000 /r /n  -> break
	if(sz_char == ' ' || sz_char == '\t') {cur_ptr++;continue;}
	else if(sz_char == '/') 
	{
		if(下一个字符=='/') {  //跳过注释行
			while(!IS_ENDCHAR(当前字符)){
				当前字符 = m_script_text[++cur_ptr];
			}
			break;			
		}
		else if(留待多行注释/**/){
			
		}
		else if(下一个字符=='='){
			SET_TOKEN(ASS_DIV,0,0,last_ptr,cur_ptr);
			m_token_list.push_back(token);
			cur_ptr++;
		}
		else {
			SET_TOKEN('/',0,0,last_ptr,cur_ptr);
			m_token_list.push_back(token);
		}
	}
	else if(sz_char == '*')
	{
		if(下一个字符=='='){
			SET_TOKEN(ASS_MUL,0,0,last_ptr,cur_ptr);
			m_token_list.push_back(token);
			cur_ptr++;
		}
		else{
			SET_TOKEN('*',0,0,last_ptr,cur_ptr);
			m_token_list.push_back(token);
		}
	}
	//解析~,;[](){}
	else if(strchr("~,;[]{}()",sz_char))
	{
		SET_TOKEN(sz_char,0,0,last_ptr,cur_ptr);
		m_token_list.push_back(token);
		cur_ptr++;
	}
	//解析小数
	else if(sz_char == '.')
	{
		last_ptr = cur_ptr++;
		while(sz_char>='0' && sz_char <= '9')
		{
			sz_char = m_script_text[++cur_ptr];
		}
		if((last_ptr+1)>cur_ptr)
		{
			SET_TOKEN('.',0,0,last_ptr,cur_ptr);
			m_token_list.push_back(token);
		}
		else
		{
			//set_token 获取当前数值字符串存入token.ps_id
			SET_TOKEN(IDETIFIER,CONSTANT_NUM,0,last_ptr,cur_ptr-1);
			token.ps_id = new char[token.char_length+1];
			memcpy(token.ps_id,&m_script_text[last_ptr],sizeof(char)*token.char_length);
			token.ps_id[token.char_length] = 0x0000;
			m_token_list.push_back(token);
		}
	}
	//解析关系赋值等运算符
	else if(strchr("+-%^|&=<>!"),sz_char)
	{
		last_ptr = cur_ptr++;
		//解析 += -= %= ^= |= &= == <= >= !=
		if(m_script_text[cur_ptr] == '=')
		{
			switch(sz_char)
			{
				case  '+' : SET_TOKEN(ASS_ADD,0,0,last_ptr,cur_ptr);break;
				case  '-' : SET_TOKEN(ASS_SUB,0,0,last_ptr,cur_ptr);break;
				case  '%' : SET_TOKEN(ASS_MOD,0,0,last_ptr,cur_ptr);break;
				case  '^' : SET_TOKEN(ASS_XOR,0,0,last_ptr,cur_ptr);break;
				case  '|' : SET_TOKEN(ASS_OR,0,0,last_ptr,cur_ptr);break;
				case  '&' : SET_TOKEN(ASS_AND,0,0,last_ptr,cur_ptr);break;
				case  '=' : SET_TOKEN(EQ,0,0,last_ptr,cur_ptr);break;
				case  '<' : SET_TOKEN(LOE,0,0,last_ptr,cur_ptr);break;
				case  '>' : SET_TOKEN(GOE,0,0,last_ptr,cur_ptr);break;
				case  '!' : SET_TOKEN(NE,0,0,last_ptr,cur_ptr);break;
				default : break;
			}
			m_token_list.push_back(token);
			cur_ptr++;
			continue;
		}
		//解析 ++ -- || && << >> <<= >>=
		if((sz_char == '+' && m_script_text[cur_ptr] == '+') ||
		  (sz_char == '-' && m_script_text[cur_ptr] == '-') ||
		  (sz_char == '|' && m_script_text[cur_ptr] == '|') ||
		  (sz_char == '&' && m_script_text[cur_ptr] == '&') ||
		  (sz_char == '<' && m_script_text[cur_ptr] == '<') ||
		  (sz_char == '>' && m_script_text[cur_ptr] == '>'))
		{
			switch(sz_char)
			{
				case '+' : SET_TOKEN(POSTFIX_ADD,0,0,last_ptr,cur_ptr);break;
				case '-' : SET_TOKEN(POSTFIX_ADD,0,0,last_ptr,cur_ptr);break;
				case '|' : SET_TOKEN(OROR,0,0,last_ptr,cur_ptr);break;
				case '&' : SET_TOKEN(ANDAND,0,0,last_ptr,cur_ptr);break;
				default : break;
			}
			if(m_script_text[cur_ptr+1] == '=')
			{
				if(sz_char == '<')
				{
					SET_TOKEN(ASS_SHL,0,0,last_ptr,cur_ptr);
				}
				else
				{
					SET_TOKEN(ASS_SHR,0,0,last_ptr,cur_ptr);
				}
			}
			else
			{
			    if(sz_char == '<')
				{
					SET_TOKEN(SHL,0,0,last_ptr,cur_ptr);
				}
				else
				{
					SET_TOKEN(SHR,0,0,last_ptr,cur_ptr);
				}
			}
			m_token_list.push_back(token);
			cur_ptr++;
			continue;
		}
		else
		{
			SET_TOKEN(sz_char,0,0,last_ptr,last_ptr);
			m_token_list.push_back(token);
			cur_ptr++;
		}
	}
	//解析常数分为10进制，16进制
	else if(sz_char >= '0' && sz_char <= '9')
	{
		last_ptr = cur_ptr++;
		if(sz_char == '0' && (m_script_text[cur_ptr] == 'x' || m_script_text[cur_ptr] == 'X')
		{
			sz_char = m_script_text[++cur_ptr];
			while(sz_char >= '0' && sz_char <= '9')
			{
				sz_char = m_script_text[++cur_ptr];
			}
			SET_TOKEN(IDETIFIER,CONSTANT_NUM,0,last_ptr,cur_ptr - 1);
			token.ps_id = new char[token.char_length+1];
			memcpy(token.ps_id,&m_script_text[last_ptr],sizeof(char)*token.char_length);
			token.ps_id[token.char_length] = 0x0000;
			m_token_list.push_back(token);
		}
		else
		{
			sz_char = m_script_text[++cur_ptr];
			while(sz_char >= '0' && sz_char <= '9')
			{
				sz_char = m_script_text[++cur_ptr];
			}
			if(sz_char == '.')
			{
				while(sz_char >= '0' && sz_char <= '9')
				{
					sz_char = m_script_text[++cur_ptr];
				}
			}
			SET_TOKEN(IDETIFIER,CONSTANT_NUM,0,last_ptr,cur_ptr - 1);
			token.ps_id = new char[token.char_length+1];
			memcpy(token.ps_id,&m_script_text[last_ptr],sizeof(char)*token.char_length);
			token.ps_id[token.char_length] = 0x0000;
			m_token_list.push_back(token);
		}
	}
	//解析“”中字符串 常量字符串
	else if(sz_char == '"')
	{
		last_ptr = cur_ptr++;
		sz_char = m_script_text[cur_ptr];
		while(!IS_ENDCHAR(sz_char) || sz_char != '"')
		{
			if(sz_char == '\\')
			{
				if(m_script_text[cur_ptr+1] == '\\' || m_script_text[cur_ptr+1] == '"')
					cur_ptr++;	
			}
			sz_char = m_script_text[++cur_ptr];
		}
		if(sz_char != '"')
		{
			//报告错误
		}
		SET_TOKEN(IDETIFIER,CONSTANT_STRING,0,last_ptr,cur_ptr - 1);
		if(last_ptr+1 >= cur_ptr-1)
		{
			//空字符串""
			token.ps_id = new char[1];
			token.ps_id[0] = 0x0000;
		}
		else
		{
			token.ps_id = new char[token.char_length+1];
			memcpy(token.ps_id,&m_script_text[last_ptr],sizeof(char)*token.char_length);
			token.ps_id[token.char_length] = 0x0000;
		}
		//转换转义字符
		convert_escape_character(token.ps_id);
		m_token_list.push_back(token);
		cur_ptr++
	}
	//解析其他IDETIFIER
	else if(sz_char == '_' || sz_char == '$' || is_chinese_character(sz_char) || 
	(sz_char >= 'a' && sz_char <= 'z') || (sz_char >= 'A' && sz_char <= 'Z') ||
	IS_VALID_OBJECT_NAME_HEAD(sz_char))
	{
		
	}
	//其他情况
	else
	{
		//存储解析信息，报错
	}
}

void convert_escape_character(char* tokenString)
{
	char* cur_ptr = tokenString;
	int l_ptrLast = strlen(tokenString);
	bool isNeedTrans = false;
	while(cur_ptr!=tokenString+strlen(tokenString)-1)
	{
		if(*cur_ptr == '\\')
		{

			if(*cur_ptr == '\\')
			{
				isNeedTrans = true;
			}
			if(*cur_ptr == 't')
			{
				*(cur_ptr-1) = '\t';
				isNeedTrans = true;		
			}
			if(*cur_ptr == 'r')
			{
				*(cur_ptr-1) = '\r';
				isNeedTrans = true;		
			}
			if(*cur_ptr == 'n')
			{
				*(cur_ptr-1) = '\n';
				isNeedTrans = true;		
			}
			if(*cur_ptr == '"')
			{
				*(cur_ptr-1) = '"';
				isNeedTrans = true;		
			}
			if(isNeedTrans)
			{
				for(int i = cur_ptr - tokenString;i<strlen(tokenString);i++)
				{
					tokenString[i] = tokenString[i+1];
				}
				l_ptrLast--;
			}
		}
		cur_ptr++;
	}
	tokenString[l_ptrLast+1] = 0x0000;
}














