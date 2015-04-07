#------------------------------------------编辑器--------------------------------------------------------

#c++编译工具gcc,g++,clang,clang++
CC = g++ 

#------------------------------------------编辑器End--------------------------------------------------------

#------------------------------------------目录--------------------------------------------------------

#依赖目标/文件查找目录
VPATH = $(OBJ_OUTPUT_DIR) 

#输出目录
OBJ_OUTPUT_DIR=./output
OBJ_OUTPUT=./output
$(shell mkdir $(OBJ_OUTPUT_DIR))
$(shell mkdir $(OUTPUT_DIR))

#源文件目录
SOURCE_DIR=

#库主目录
LIB_HOME=../

#库头文件目录
HEADER_DIR=$(LIB_HOME)/Include

#------------------------------------------目录End--------------------------------------------------------

#------------------------------------------编译选项--------------------------------------------------------

#SO文件编译选项
CFLAGS= -O -g -fPIC -Wall -Wno-strict-aliasing -D_REENTRANT -DUSE_APACHE -DNO_STRING_CIPHER 

#警告级别
WARNING_LEVEL += -O3 

#头文件目录：-I 目录
INCLUDE = -I. -I$(HEADER_DIR) 

#库目录，库文件:-L 目录 -库名
#SYSLIB = -lnsl -lc -lm -lpthread -lstdc++ 
LIB = -lnsl -lc -lm -pthread -lstdc++ 

#静态库：.a文件路径名
#LIB += $(LIB_HOME)/lib/ThreadLib.a 

#------------------------------------------编译选项End--------------------------------------------------------


#项目入口主文件
MAIN =


#------------------------------------------输出--------------------------------------------------------
#编译产生的程序文件名
OUTPUT = demo

#目标文件
OBJ_PRO = $(notdir $(patsubst %.cpp,%.o,$(wildcard *.cpp))) 

#依赖的目标文件
DEPENDENCE = $(OBJ_PRO) 

#连接项目，需要的所有其它源文件或目标文件(带目录)
OBJ = $(addprefix $(OBJ_OUTPUT_DIR)/, $(OBJ_PRO)) 

#------------------------------------------输出End--------------------------------------------------------


$(OBJ_OUTPUT)/$(OUTPUT):$(MAIN)$(DEPENDENCE)
	$(CC) -o $@ $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE)$(MAIN)$(OBJ)$(LIB)


$(OBJ_PRO):%.o:%.cpp %.h
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $*.cpp



#------------------------------------------清理重新编译----------------------------------------------------
clean:
	-rm -f $(OBJ_OUTPUT)/$(OUTPUT) $(OBJ_OUTPUT_DIR)/*.o
	
.PHONY: clean

