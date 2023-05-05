#include "SDBlockDevice.h"
#include "FATFileSystem.h"
#include "PushSwitch.hpp"
#include <chrono>
#include "mbed.h"
#include <chrono>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <math.h>
#include <cmath>
#include <iterator>

using namespace std;

struct pdData {
    unsigned short acRead;
    unsigned short dcRead; 
    uint32_t crcbox;
};
struct pdData readData;
struct pdData extract;

//Buffers for storing angle data for DFT
pdData buffer_1[1000];
pdData buffer_2[1000];

int bufferFlag=0;
int sampleCounter=0;
int axisChoice=0;
int amount=1;
int stop=1;
Timer tmr1;

PushSwitch btnBlue(PC_13);

Mutex sdLock;

MbedCRC<POLY_32BIT_ANSI, 32> ct;
uint32_t crc;

Mail<pdData, 32> mail_box;

EventQueue mainQueue;
EventQueue pwmQueue;
EventQueue pdReadQueue;
EventQueue bufferQueue;
EventQueue sdWriteQueue;
EventQueue printQueue;
EventQueue errorQueue;

Thread pwm;
Thread pdRead;
Thread buffer;
Thread sdWrite;
Thread prints;
Thread errors;

DigitalOut pwmControl(PA_8, 1);
DigitalOut iLED(PA_0, 1);
AnalogIn acOutput(PC_1);
AnalogIn dcOutput(PC_0);
//SDBlockDevice sd;

int writeSDCard(pdData sendData[1000]);
// PB_5    MOSI (Master Out Slave In)
// PB_4    MISO (Master In Slave Out)
// PB_3    SCLK (Serial Clock)
// PC_7    CS (Chip Select)
SDBlockDevice sd(PB_5, PB_4, PB_3, PC_7); //SD card object 


void pwmTask () {
    pwmQueue.dispatch_forever();
}

void pdReadTask () {
    pdReadQueue.dispatch_forever();
}

void bufferTask () {
    bufferQueue.dispatch_forever();
}

void sdWriteTask () {
    sdWriteQueue.dispatch_forever();
}

void printTask () {
    printQueue.dispatch_forever();
}

void errorTask () {
    errorQueue.dispatch_forever();
}

void consumer();

void errorHandler () {
    pwm.terminate();
    pdRead.terminate();
    sdWrite.terminate();
    prints.terminate();

    error("Error Has Happend!");
    ThisThread::sleep_for(5s);
    system_reset();
}

void pwmSwitch() {
    pwmControl=!pwmControl;
}

void pdReading() {
    readData.acRead = acOutput.read_u16();
    readData.dcRead = dcOutput.read_u16();

    //printQueue.call(printf, "AC Data %u\n", readData.acRead);
    
    pdData* message = mail_box.try_alloc_for(2s);

    if (message == NULL) {
        errorQueue.call(errorHandler);
        return;
    }

    message->acRead = readData.acRead;
    message->dcRead = readData.dcRead;

    osStatus stat = mail_box.put(message);

    if (stat != osOK) {
        //ERROR
        printf("mail_box.put() Error code: %4Xh, Resource not available\r\n", stat);  
        mail_box.free(message);
        errorQueue.call(errorHandler);
        return;
    }

    ct.compute((void *)&mail_box, 32, &crc);
    printQueue.call(printf, "The CRC of data \"Mail\" is : 0x%x\n", crc);

    bufferQueue.call(consumer);
}

void consumer() {

    pdData* payload;

    payload = mail_box.try_get_for(2s);

    extract.acRead = payload->acRead;
    extract.dcRead = payload->dcRead;
    extract.crcbox = payload->crcbox;

    unsigned short acTemp = extract.acRead;
    unsigned short dcTemp = extract.dcRead;

    mail_box.free(payload);

    //Write values to empty buffer
    switch (bufferFlag) {
        case 0:
            buffer_1[sampleCounter].acRead = acTemp;
            buffer_1[sampleCounter].dcRead = dcTemp;
            break;
        case 1:
            buffer_2[sampleCounter].acRead = acTemp;
            buffer_2[sampleCounter].dcRead = dcTemp;
            break;
        default:
            errorQueue.call(errorHandler);
            break;
        }

        sampleCounter++;

        if (sampleCounter==1000) {
            uint32_t crc;
            printQueue.call(printf, "Switching buffer...\n");
            switch (bufferFlag){
                case 0:
                    bufferFlag=1;
                    tmr1.stop();
                    printQueue.call(printf,"Time taken: %llu milliseconds\n", duration_cast<std::chrono::milliseconds>(tmr1.elapsed_time()).count()); //Record time taken between switching buffers (should always be 2s)
                    printQueue.call(printf, "Starting data send on buffer 1...\n");
                    tmr1.reset();
                    tmr1.start();
                    sdWriteQueue.call(writeSDCard, buffer_1);
                    memset(buffer_1, 0xFF, 1000);
                    //sdCheck.compute((void *)buffer_1, 1000, &crc);
                    //printQueue.call(printf, "16BIT SD CRC (1000 bytes 0xFF) is \"0x7FA1\" Result: 0x%lx\n", crc);
                    //memset
                    break;
                case 1:
                    bufferFlag=0;
                    tmr1.stop();
                    printQueue.call(printf,"Time taken: %llu milliseconds\n", duration_cast<std::chrono::milliseconds>(tmr1.elapsed_time()).count()); //Record time taken between switching buffers (should always be 2s)
                    printQueue.call(printf, "Starting data send on bufffer 2...\n");
                    tmr1.reset();
                    tmr1.start();
                    sdWriteQueue.call(writeSDCard, buffer_2); //Start DFT
                    memset(buffer_2, 0xFF, 1000);
                    //sdCheck.compute((void *)buffer_2, 1000, &crc);
                    //printQueue.call(printf, "16BIT SD CRC (1000 bytes 0xFF) is \"0x7FA1\" Result: 0x%lx\n", crc);
                    break;
                default:
                    errorQueue.call(errorHandler);
                    break;
            }
            sampleCounter=0;
        }

}

int writeSDCard(pdData sendData[1000]) {  
    
    printf("Initialise and write to a file\n");
    int err;
    err=sd.init();
    if ( 0 != err) {
        printf("Init failed %d\n",err);
        return -1;
    }
    
    FATFileSystem fs("sd", &sd); 
    
    /*
    //If anything other than 0 is passed as a param to the function then remove old data from the txt file. Used to clean the file when the program starts.
    if (clearFlag!=0){
        FILE *fp = fopen("/sd/results.txt","w"); //Open in write mode and write no characters to clear the file
        fprintf(fp, "");
        fclose(fp);
        sd_card.deinit();
        return 0;
    } 
    */
    printQueue.call(printf, "SD init succeded, writing to sd...\n");

    //Set format and attempt to open file in append mode
    FILE *fp = fopen("/sd/glucoseresults.txt","a+");
 
    //If unable to open file then a critical error has been encountered as data has not been written and will be lost. 
    if(fp == NULL) {   
        errorQueue.call(errorHandler);
        return -1;
    } 
    //If file is opened successfully then write data to sd card. 
    else {
        bool lockTaken = sdLock.trylock_for(200ms); //Code is event driven and the DFT() function modifies resultArray before calling writeSDcard() but acquire lock anyway as overhead is not significant
        
        if (lockTaken == true) {
            for(int i=0; i<1000; i++) {
                fprintf(fp, "%u,%u\n", sendData[i].acRead, sendData[i].dcRead); //Write each result is not significant
            }
            sdLock.unlock(); //Release lock as finsihed accessing global variable
        }
        else {
            errorQueue.call(errorHandler); //If not able to acquire lock then deadlock has occured so reset system
            return -1;
        }

        fprintf(fp, "\n\n");
        fclose(fp); 
        printQueue.call(printf, "SD Write done...\n");
        printQueue.call(printf, "Data set %i saved to the SD card!\n\n", amount);
        sd.deinit();
        
            if (amount==stop) {
                pwm.terminate();
                pdRead.terminate();
                buffer.terminate();
                //sdWrite.terminate();
                //prints.terminate();
                //errors.terminate();
                printQueue.call(printf,"Sampling Complete!\n");
                printQueue.call(printf,"Please remove the SD card to review sampled data. The file is called 'glucoseresults'.\n");
                printQueue.call(printf,"System Restarting in 5 seconds!\n\n");
                tmr1.stop();
                tmr1.reset();
                tmr1.start();
                while(duration_cast<std::chrono::milliseconds>(tmr1.elapsed_time()).count() < 5000) {}
                system_reset();

            }
            else {
                amount++;
            }
            return 0;
    }

    
}

int memoryReset() {
    int sd_check = sd.init(); //Check if sd card is connected, return 0 if connected
    
    //If sd card is connected then clear the contents of the results text file
    if(sd_check==0) {
        //If anything other than 0 is passed as a param to the function then remove old data from the txt file. Used to clean the file when the program starts.

        FATFileSystem fs("sd", &sd); 

        FILE *fp = fopen("/sd/glucoseresults.txt","w"); //Open in write mode and write no characters to clear the file
        fprintf(fp, "");
        fclose(fp);
        sd.deinit();
        printf("Memory Cleared Successfully!\n");
        return 0;
    }
    else {
        printf("ERROR: SD Init Failed - Data Will Not Be Saved!\n");
        return -1;
    }
}   
 
int main () 
{
    bool confirmed = false;

    cout << "Welcome to Blood Glucose Sampling using PPG signals!\n";
    cout << "WARNING: An SD card must be inserted to use this program!\n";
    cout << "Once an SD card has be inserted, please press the blue button to wipe previous data on the SD Card.\n";
    
    btnBlue.waitForPress();
    ThisThread::sleep_for(50ms);

    cout << "Thank you! Memory wipe in progress...\n";
    memoryReset();
    //cout << "Memory wipe complete!\n";
    
    cout << "A new test can begin.\n";
    cout << "How many 10 second samples are required? Please type the amount in the terminal and press the enter key.\n";
    cin >> stop;
    cout << stop << " lots of 10 second samples have been chosen.\n";

    cout << "Please press the blue button to start sampling!\n";

    btnBlue.waitForPress();
    ThisThread::sleep_for(50ms);
    cout << "Sampling starting...\n\n";

    pwm.start(pwmTask);
    pwm.set_priority(osPriorityRealtime); //Set thread 1 priority to highest to ensure accurate sampling with no jitter
    pdRead.start(pdReadTask);
    buffer.start(bufferTask);
    sdWrite.start(sdWriteTask);
    prints.start(printTask);
    errors.start(errorTask);
    //prints.set_priority(osPriorityBelowNormal); //Set thread 4 priority to low to ensure DFT only uses spare CPU capacity so sampling process is prioritiesd.
    //printQueue.call(printf, "Starting...\n");

    pwmQueue.call_every(1ms, callback(pwmSwitch)); //Call the readSensors() function every 10ms on dedicated Realtime priorty thread to ensure jitter free sampling at 100Hz
    pdReadQueue.call_every(10ms, callback(pdReading));

    mainQueue.dispatch_forever();
}


