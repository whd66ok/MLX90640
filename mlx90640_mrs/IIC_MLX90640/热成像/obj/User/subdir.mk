################################################################################
# MRS Version: 1.9.1
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Displaytemp.c \
../User/ch32v30x_it.c \
../User/iic.c \
../User/lcd.c \
../User/main.c \
../User/system_ch32v30x.c 

OBJS += \
./User/Displaytemp.o \
./User/ch32v30x_it.o \
./User/iic.o \
./User/lcd.o \
./User/main.o \
./User/system_ch32v30x.o 

C_DEPS += \
./User/Displaytemp.d \
./User/ch32v30x_it.d \
./User/iic.d \
./User/lcd.d \
./User/main.d \
./User/system_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\gitprojiect\MLX90640\mlx90640_mrs\IIC_MLX90640\�ȳ���\Debug" -I"D:\gitprojiect\MLX90640\mlx90640_mrs\IIC_MLX90640\�ȳ���\Core" -I"D:\gitprojiect\MLX90640\mlx90640_mrs\IIC_MLX90640\�ȳ���\User" -I"D:\gitprojiect\MLX90640\mlx90640_mrs\IIC_MLX90640\�ȳ���\Peripheral\inc" -I"D:\gitprojiect\MLX90640\mlx90640_mrs\IIC_MLX90640\�ȳ���\mlx9640" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

