#include <dc_motor_library.h>

//void set_register(int8 *motor_values)
//{
//    int counter = 7;
//     while (counter>=0)
//     {
//         if (!Clock_Read())
//         {
//            Data_Write(motor_values[counter]);
//            Fix_Write(0);
//            Clock_Write(1);
//            CyDelay(20);
//         }
//         else
//         {
//            Clock_Write(0);
//            Fix_Write(1);
//            counter--;
//         }
//    }
//}

/**  for case with DC motor defines witch motor will work and also defines direction
    Write to serial (looks like SPI) port expender
        - 4 bytes does to first motor controller (2 H bridge control) based on L293D
        - 4 bytes does to second motor controller (2 H bridge control) based on L293D
    all that left is to use PWM for controlling motor enabled signal
    */
void writeShiftRegisters(uint8_t shift_reg)
{
    uint8_t i;
    Pin_OE_Write(0);
    Pin_SER_Write(0);
    for (i=0; i<8; i++)
    {
        Pin_SER_Write(0x01&shift_reg);
        shift_reg>>=1;
        Pin_SRCLK_Write(1);
        Pin_RCLK_Write(0);
        CyDelay(10);
        Pin_SRCLK_Write(0);
        Pin_RCLK_Write(1);
        CyDelay(10);
    }
    Pin_OE_Write(0);
}

/*
void clear_motor_register(int8 *motor)
{
    int8 counter = 7;
     while (counter!=0)
     {
         if (!Clock_Read())
         {
            Data_Write(motor[counter]);
            Fix_Write(0);
            Clock_Write(1);
            CyDelay(20);
         }
         else
         {
            Clock_Write(0);
            Fix_Write(1);
            counter--;
         }
    }
    return;
}
*/
