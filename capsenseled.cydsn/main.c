#include <project.h>
#include <dc_motor_library.h>

uint8_t registers_ahead = 0b10101010;
uint8_t registers_backward = 0b01010101;
uint8_t registers_direction = 0b10101010;

/***************************************************************
 * Function to update the LED state in the GATT database
 **************************************************************/
void updateRight()
{
    CYBLE_GATTS_HANDLE_VALUE_NTF_T 	tempHandle;
   
    uint8 red_State = !red_Read();
    
    if(CyBle_GetState() != CYBLE_STATE_CONNECTED)
        return;
    
    tempHandle.attrHandle = CYBLE_ROBOT_RIGHT_CHAR_HANDLE;
  	tempHandle.value.val = (uint8 *) &red_State;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);  
}

void updateLeft() 
{
    CYBLE_GATTS_HANDLE_VALUE_NTF_T 	tempHandle;
   
    uint8 green_State = !green_Read();
    
    if(CyBle_GetState() != CYBLE_STATE_CONNECTED)
        return;
    
    tempHandle.attrHandle = CYBLE_ROBOT_LEFT_CHAR_HANDLE;
  	tempHandle.value.val = (uint8 *) &green_State;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);  
}

void updateDirection() 
{
    CYBLE_GATTS_HANDLE_VALUE_NTF_T 	tempHandle;
    
    if(CyBle_GetState() != CYBLE_STATE_CONNECTED)
        return;
    
    tempHandle.attrHandle = CYBLE_ROBOT_DEFINE_DIRECTION_CHAR_HANDLE;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);  
}

void updateBothMotors() 
{
    CYBLE_GATTS_HANDLE_VALUE_NTF_T 	tempHandle;

    if(CyBle_GetState() != CYBLE_STATE_CONNECTED)
        return;
    
    tempHandle.attrHandle = CYBLE_ROBOT_BOTH_MOTORS_CHAR_HANDLE;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);  
}

/***************************************************************
 * Function to handle the BLE stack
 **************************************************************/
void BleCallBack(uint32 event, void* eventParam)
{
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;

    switch(event)
    {
        /* if there is a disconnect or the stack just turned on from a reset then start the advertising and turn on the LED blinking */
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            pwm_Start();
        break;
        
        /* when a connection is made, update the LED and Capsense states in the GATT database and stop blinking the LED */    
        case CYBLE_EVT_GATT_CONNECT_IND:
//            updateRight();
            updateLeft();
            updateDirection();
            updateBothMotors();
            pwm_Stop();
		break;

        /* handle a write request */
        case CYBLE_EVT_GATTS_WRITE_REQ:
            wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam;
			      
            /* request write the LED value */
            if(wrReqParam->handleValPair.attrHandle == CYBLE_ROBOT_RIGHT_CHAR_HANDLE)
            {
                /* only update the value and write the response if the requested write is allowed */
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
                {
                    if(!wrReqParam->handleValPair.value.val[0]) // off
                    {
                    red_Write(!wrReqParam->handleValPair.value.val[0]);
                    CyBle_GattsWriteRsp(cyBle_connHandle);

                    } else {                                    // on
                        red_Write(!wrReqParam->handleValPair.value.val[0]);
                    CyBle_GattsWriteRsp(cyBle_connHandle);
                          // -------------right motor          
                    CyDelay(1000);
                    writeShiftRegisters(registers_direction);   // define direction
                    Pin_l293_IC1_EN_12_Write(1);
                    Pin_l293_IC1_EN_34_Write(0);
                    CyDelay(500);
                    Pin_l293_IC1_EN_12_Write(0);                // turns off left motor
//                  CyDelay(200);                               // delay between motors stop
                    Pin_l293_IC1_EN_34_Write(0);                // turns off right motor
                    }   
                    
                }
            }
             if(wrReqParam->handleValPair.attrHandle == CYBLE_ROBOT_LEFT_CHAR_HANDLE)
            {
                /* only update the value and write the response if the requested write is allowed */
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
                {
                    
                    if(!wrReqParam->handleValPair.value.val[0])
                    {
                    green_Write(!wrReqParam->handleValPair.value.val[0]);
                    CyBle_GattsWriteRsp(cyBle_connHandle);
                    }
                    else
                    {
                    green_Write(!wrReqParam->handleValPair.value.val[0]);
                    CyBle_GattsWriteRsp(cyBle_connHandle);
                    
                         // -------------left motor
                    CyDelay(1000);
                    writeShiftRegisters(registers_direction);   // define direction
                    Pin_l293_IC1_EN_12_Write(0);
                    Pin_l293_IC1_EN_34_Write(1);
                    CyDelay(500);
                    Pin_l293_IC1_EN_12_Write(0);                // turns off left motor
                    //CyDelay(200);                             // delay between motors stop
                    Pin_l293_IC1_EN_34_Write(0);                // turns off right motor
                    }
                }
            
            }
            
             if(wrReqParam->handleValPair.attrHandle == CYBLE_ROBOT_BOTH_MOTORS_CHAR_HANDLE)
            {
                /* only update the value and write the response if the requested write is allowed */
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
                {
                    if(!wrReqParam->handleValPair.value.val[0])
                    {
                    CyBle_GattsWriteRsp(cyBle_connHandle);
                    }
                    else
                    {
                    CyBle_GattsWriteRsp(cyBle_connHandle);
                    
                         // -------------all motors
                    CyDelay(1000);
                    writeShiftRegisters(registers_direction);   // defines direction
                    Pin_l293_IC1_EN_12_Write(1);
                    Pin_l293_IC1_EN_34_Write(1);
                    CyDelay(500);
                    Pin_l293_IC1_EN_12_Write(0);                // turns off left motor
                    //CyDelay(200);                             // delay between stops
                    Pin_l293_IC1_EN_34_Write(0);                // turns off right motor
                    }
                }
            
            }
            
             if(wrReqParam->handleValPair.attrHandle == CYBLE_ROBOT_DEFINE_DIRECTION_CHAR_HANDLE)
            {
                                /* only update the value and write the response if the requested write is allowed */
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
                {
                    if(!wrReqParam->handleValPair.value.val[0]) // off
                    {
                        registers_direction = registers_backward;
                        CyBle_GattsWriteRsp(cyBle_connHandle);
                    } else {                                    // on
                        registers_direction = registers_ahead;
                        CyBle_GattsWriteRsp(cyBle_connHandle);
                    }
                }
            }
			break;  
        
        default:
            break;
    }
} 

/***************************************************************
 * Main
 **************************************************************/
int main()
{
    CyGlobalIntEnable; 
    writeShiftRegisters(0x00);
    Pin_l293_IC1_EN_12_Write(1);   /* Start all motors */
    Pin_l293_IC1_EN_34_Write(1);
    Pin_l293_IC2_EN_12_Write(1);
    Pin_l293_IC2_EN_34_Write(1);

    /* Start BLE stack and register the callback function */
    CyBle_Start(BleCallBack);

    for(;;)
    {   
        CyBle_ProcessEvents();
        CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);    
    }
}
