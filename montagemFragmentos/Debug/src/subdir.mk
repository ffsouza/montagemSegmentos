################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/fragment.cpp \
../src/fragmentBuilder.cpp \
../src/montagemFragmentos.cpp \
../src/params.cpp 

OBJS += \
./src/fragment.o \
./src/fragmentBuilder.o \
./src/montagemFragmentos.o \
./src/params.o 

CPP_DEPS += \
./src/fragment.d \
./src/fragmentBuilder.d \
./src/montagemFragmentos.d \
./src/params.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I.././include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


