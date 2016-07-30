
_main:

;Test.c,23 :: 		void main()
;Test.c,25 :: 		ANSELC = 0;                      // Configure AN pins as digital
	CLRF        ANSELC+0 
;Test.c,26 :: 		SLRCON = 0;                      // Configure all PORTS at the standard Slew Rate
	CLRF        SLRCON+0 
;Test.c,30 :: 		SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV16,_SPI_DATA_SAMPLE_MIDDLE,
	MOVLW       1
	MOVWF       FARG_SPI1_Init_Advanced_master+0 
	CLRF        FARG_SPI1_Init_Advanced_data_sample+0 
;Test.c,31 :: 		_SPI_CLK_IDLE_LOW, _SPI_LOW_2_HIGH);
	CLRF        FARG_SPI1_Init_Advanced_clock_idle+0 
	MOVLW       1
	MOVWF       FARG_SPI1_Init_Advanced_transmit_edge+0 
	CALL        _SPI1_Init_Advanced+0, 0
;Test.c,34 :: 		if (Mmc_Fat_Init() == 0) {
	CALL        _Mmc_Fat_Init+0, 0
	MOVF        R0, 0 
	XORLW       0
	BTFSS       STATUS+0, 2 
	GOTO        L_main0
;Test.c,36 :: 		SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV4, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_LOW_2_HIGH);
	CLRF        FARG_SPI1_Init_Advanced_master+0 
	CLRF        FARG_SPI1_Init_Advanced_data_sample+0 
	CLRF        FARG_SPI1_Init_Advanced_clock_idle+0 
	MOVLW       1
	MOVWF       FARG_SPI1_Init_Advanced_transmit_edge+0 
	CALL        _SPI1_Init_Advanced+0, 0
;Test.c,38 :: 		Mmc_Fat_Assign(&filename, 0x80);          // Find existing file or create a new one
	MOVLW       _filename+0
	MOVWF       FARG_Mmc_Fat_Assign_filename+0 
	MOVLW       hi_addr(_filename+0)
	MOVWF       FARG_Mmc_Fat_Assign_filename+1 
	MOVLW       128
	MOVWF       FARG_Mmc_Fat_Assign_file_cre_attr+0 
	CALL        _Mmc_Fat_Assign+0, 0
;Test.c,39 :: 		Mmc_Fat_Rewrite();
	CALL        _Mmc_Fat_Rewrite+0, 0
;Test.c,41 :: 		Mmc_Fat_Write(txt,21);
	MOVLW       _txt+0
	MOVWF       FARG_Mmc_Fat_Write_fdata+0 
	MOVLW       hi_addr(_txt+0)
	MOVWF       FARG_Mmc_Fat_Write_fdata+1 
	MOVLW       21
	MOVWF       FARG_Mmc_Fat_Write_data_len+0 
	MOVLW       0
	MOVWF       FARG_Mmc_Fat_Write_data_len+1 
	CALL        _Mmc_Fat_Write+0, 0
;Test.c,42 :: 		}
L_main0:
;Test.c,44 :: 		for(;;);
L_main1:
	GOTO        L_main1
;Test.c,45 :: 		}
	GOTO        $+0
; end of _main
