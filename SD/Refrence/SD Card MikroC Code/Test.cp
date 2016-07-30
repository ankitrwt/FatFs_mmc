#line 1 "C:/Documents and Settings/1007805/My Documents/pic flowcode/MikroC/SD Card MikroC Code/Test.c"
#line 15 "C:/Documents and Settings/1007805/My Documents/pic flowcode/MikroC/SD Card MikroC Code/Test.c"
char filename[] = "Test.TXT";
unsigned char txt[] = "Writing to SD Card...";


sbit Mmc_Chip_Select at LATC2_bit;
sbit Mmc_Chip_Select_Direction at TRISC2_bit;


void main()
{
 ANSELC = 0;
 SLRCON = 0;



SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV16,_SPI_DATA_SAMPLE_MIDDLE,
_SPI_CLK_IDLE_LOW, _SPI_LOW_2_HIGH);


 if (Mmc_Fat_Init() == 0) {

 SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV4, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_LOW_2_HIGH);

 Mmc_Fat_Assign(&filename, 0x80);
 Mmc_Fat_Rewrite();

 Mmc_Fat_Write(txt,21);
 }

 for(;;);
}
