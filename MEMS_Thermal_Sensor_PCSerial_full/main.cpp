/*参照：http://www.omron.co.jp/ecb/products/sensor/special/mems/pdf/AN-D6T-01JP_r2.pdf*/
#include "mbed.h"

I2C i2c(p9, p10); // sda, scl

Serial pc(USBTX, USBRX);        // tx, rx
 
#define  D6T_addr  0x14
#define  D6T_cmd   0x4c
 
int main() {
    char  I2C_rd[64]; // 生データ
    short  datr[16]; // 16点 温度データ（10倍整数）
    short  PTAT; // センサ内部PTAT温度データ（10倍整数）
    double   dt[16]; // 16点 温度データ
    short   d_PTAT; // センサ内部PTAT温度データ
    int  i,j;
    int  itemp;
    
    while(1) {
        //// measure
        i2c.start();
        i2c.write(D6T_addr);
        i2c.write(D6T_cmd);
        // Repeated Start condition
        i2c.read(D6T_addr,I2C_rd,35);
//        if(check_PEC(I2C_rd) == -1) continue; // error
        for(i=0,j=0;i<17;i++){
            itemp = (I2C_rd[j++] & 0xff);
            itemp += I2C_rd[j++] * 256;
            if(i == 0) PTAT = itemp;
            else datr[i-1] = itemp;
        }
        for(i=0;i<16;i++){
            dt[i] = 0.1 * datr[i];
            if(i%4==0) pc.printf("\n");
            pc.printf("%3.1f ",dt[i]);
        }
        pc.printf("\n\n");
        d_PTAT = 0.1 * PTAT;
        wait(0.3); // Loop wait
    }
}

