#------------------------------------------�༭��--------------------------------------------------------

#c++���빤��gcc,g++,clang,clang++
CC = g++ 

#------------------------------------------�༭��End--------------------------------------------------------

#------------------------------------------Ŀ¼--------------------------------------------------------

#����Ŀ��/�ļ�����Ŀ¼
VPATH = $(OBJ_OUTPUT_DIR) 

#���Ŀ¼
OBJ_OUTPUT_DIR=./output
OBJ_OUTPUT=./output
$(shell mkdir $(OBJ_OUTPUT_DIR))
$(shell mkdir $(OUTPUT_DIR))

#Դ�ļ�Ŀ¼
SOURCE_DIR=

#����Ŀ¼
LIB_HOME=../

#��ͷ�ļ�Ŀ¼
HEADER_DIR=$(LIB_HOME)/Include

#------------------------------------------Ŀ¼End--------------------------------------------------------

#------------------------------------------����ѡ��--------------------------------------------------------

#SO�ļ�����ѡ��
CFLAGS= -O -g -fPIC -Wall -Wno-strict-aliasing -D_REENTRANT -DUSE_APACHE -DNO_STRING_CIPHER 

#���漶��
WARNING_LEVEL += -O3 

#ͷ�ļ�Ŀ¼��-I Ŀ¼
INCLUDE = -I. -I$(HEADER_DIR) 

#��Ŀ¼�����ļ�:-L Ŀ¼ -����
#SYSLIB = -lnsl -lc -lm -lpthread -lstdc++ 
LIB = -lnsl -lc -lm -pthread -lstdc++ 

#��̬�⣺.a�ļ�·����
LIB += $(LIB_HOME)/lib/ThreadLib.a 

#------------------------------------------����ѡ��End--------------------------------------------------------


#��Ŀ������ļ�
MAIN =


#------------------------------------------���--------------------------------------------------------
#��������ĳ����ļ���
OUTPUT = demo

#Ŀ���ļ�
OBJ_PRO = $(notdir $(patsubst %.cpp,%.o,$(wildcard *.cpp))) 

#������Ŀ���ļ�
DEPENDENCE = $(OBJ_PRO) 

#������Ŀ����Ҫ����������Դ�ļ���Ŀ���ļ�(��Ŀ¼)
OBJ = $(addprefix $(OBJ_OUTPUT_DIR)/, $(OBJ_PRO)) 

#------------------------------------------���End--------------------------------------------------------


$(OBJ_OUTPUT)/$(OUTPUT):$(MAIN)$(DEPENDENCE)
	$(CC) -o $@ $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE)$(MAIN)$(OBJ)$(LIB)


$(OBJ_PRO):%.o:%.cpp %.h
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $*.cpp



#------------------------------------------�������±���----------------------------------------------------
clean:
	-rm -f $(OBJ_OUTPUT)/$(OUTPUT) $(OBJ_OUTPUT_DIR)/*.o
	
.PHONY: clean
