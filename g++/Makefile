#------------------------------------------�༭��--------------------------------------------------------

#c++���빤��gcc,g++,clang,clang++
CC = g++ 

#------------------------------------------�༭��End--------------------------------------------------------

#------------------------------------------Ŀ¼--------------------------------------------------------

#����Ŀ��/�ļ�����Ŀ¼
VPATH = $(OBJ_OUTPUT_DIR) $(SOURCE_DIR) $(HEADER_DIR) 

#OBJ���Ŀ¼
OBJ_OUTPUT_DIR=./output

#�����Ŀ¼
OUTPUT_DIR=../lib

#����Ŀ¼
$(shell mkdir $(OBJ_OUTPUT_DIR))
$(shell mkdir $(OUTPUT_DIR))

#Դ�ļ�Ŀ¼
SOURCE_DIR=../Source

#ͷ�ļ�Ŀ¼
HEADER_DIR=../Include

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
LIB += 

#------------------------------------------����ѡ��End--------------------------------------------------------

#��Ŀ������ļ�
MAIN =

#------------------------------------------���--------------------------------------------------------
#��������ĳ����ļ���
OUTPUT = ThreadLib.a

#Ŀ���ļ�
OBJ_LIB = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(SOURCE_DIR)/*.cpp))) 

#������Ŀ���ļ�
DEPENDENCE = $(OBJ_LIB) 

#������Ŀ����Ҫ����������Դ�ļ���Ŀ���ļ�(��Ŀ¼)
OBJ = $(addprefix $(OBJ_OUTPUT_DIR)/, $(OBJ_LIB)) 

#------------------------------------------���End--------------------------------------------------------

$(OUTPUT_DIR)/$(OUTPUT):$(MAIN)$(DEPENDENCE)
	ar -r $@ $(OBJ)


$(OBJ_LIB):%.o:%.cpp %.h
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $(SOURCE_DIR)/$*.cpp



#------------------------------------------�������±���----------------------------------------------------
clean:
	-rm -f $(OUTPUT_DIR)/$(OUTPUT) $(OBJ_OUTPUT_DIR)/*.o
	
.PHONY: clean