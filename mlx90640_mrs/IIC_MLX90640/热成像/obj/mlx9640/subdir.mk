################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mlx9640/MLX90640_API.c 

OBJS += \
./mlx9640/MLX90640_API.o 

C_DEPS += \
./mlx9640/MLX90640_API.d 


# Each subdirectory must supply rules for building sources it contributes
mlx9640/%.o: ../mlx9640/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\gitprojiect\MLX90640\mlx90640_mrs\IIC_MLX90640\热成像\Debug" -I"D:\gitprojiect\MLX90640\mlx90640_mrs\IIC_MLX90640\热成像\Core" -I"D:\gitprojiect\MLX90640\mlx90640_mrs\IIC_MLX90640\热成像\User" -I"D:\gitprojiect\MLX90640\mlx90640_mrs\IIC_MLX90640\热成像\Peripheral\inc" -I"D:\gitprojiect\MLX90640\mlx90640_mrs\IIC_MLX90640\热成像\mlx9640" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

