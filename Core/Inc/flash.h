// external spi flash related
void flash_sector_erase(uint32_t address_erase_s ){      // 24bit address


    uint8_t send_spi1[20]={0x90,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
   // if (address==0) return 1; // no zero
/*

	send_spi1[0]=0x05; //read status register  if writing
	send_spi1[1]=0; //24bit address msb
	status_reg[1]=1; // set busy on

	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0); //enable
	HAL_SPI_TransmitReceive(&hspi1, send_spi1, status_reg,2, 200);   // check if busy writing
	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  //disable
	if (status_reg[1]&1)   return 1 ;   // quit if busy
*/

    send_spi1[0]=0x06; //enable write  , only lasts for single operation
		HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0); // start
		HAL_SPI_Transmit(&hspi1, send_spi1, 1, 1000);       // enable write
		HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);   // end
		HAL_Delay(5);
		send_spi1[0]=0x20; //sector erase
		memcpy(&spi_send+1,&address_erase_s,3);   // copy 24bits
		HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);         // enable for sector erase   , stays empty when enabled
		HAL_SPI_Transmit(&hspi1, send_spi1, 4, 1000);   //erase sector ,works       4kbytes   (block erase=32kbytes or  64kbyte )
		HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);
		HAL_Delay(100);  // this is slow
	//	    return  0;

}

void flash_page_write(uint32_t address_write,uint8_t*  block){   // write full page (256bytes)    , or 128x 16bit audio  LSB should always be zero if 256 byte blockor it wraps around



    uint8_t send_spi1[520]={0x90,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
  //  if (address==0) return 1; // no zero



	send_spi1[0]=0x06; // 	 FIRST ENABLE WRITE , THEN SEND COMMAND TO WRITE , THEN WRITE  !!!!
			HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0); // start
			HAL_SPI_Transmit(&hspi1, send_spi1, 1, 1000);       // enable write
			HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);   // end

	send_spi1[0]=0x02; //write page
	send_spi1[1]=(address_write>>16);
	send_spi1[2]=(address_write>>8);
	send_spi1[3]=0;

	memcpy  (&send_spi1[4], block, 256);   // copy  for now
		HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);   // low
		HAL_SPI_Transmit(&hspi1, send_spi1, 260, 100);  //address,then data
		HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  // high end

		//HAL_Delay(10);  // 3ms /page

		send_spi1[0]=0x04; //disable write
		HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0); // low
		HAL_SPI_Transmit(&hspi1, send_spi1, 1, 1000);
		HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);   // high end
		//return 0;

}

void   flash_page_read (uint32_t address_read){   // returns 512 byte pointer  (256 samples)   REMINDER 2*256 BYTES HERE !!!!



        uint8_t send_spi1[520]={0x90,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
     //   if (address==0) return 1 ; // quit

	send_spi1[0]=0x03; //read page 1
	send_spi1[1]=(address_read>>16)&255;
	send_spi1[2]=(address_read>>8)&255;
	send_spi1[3]=address_read&255;     // can start anywhere



}

void  flash_block_erase(uint32_t address_erase_b ){      // 24bit address      32 kbyte


    uint8_t send_spi1[20]={0x90,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};


	send_spi1[0]=0x05; //read status register  if writing
	send_spi1[1]=0; //24bit address msb
	status_reg[1]=1; // set busy on

	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0); //enable
	HAL_SPI_TransmitReceive(&hspi1, send_spi1, status_reg,2, 200);   // check if busy writing
	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);  //disable
	if (status_reg[1]&1)   return  ;   // quit if busy

    send_spi1[0]=0x06; //enable write  , only lasts for single operation
		HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0); // start
		HAL_SPI_Transmit(&hspi1, send_spi1, 1, 1000);       // enable write
		HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);   // end
		HAL_Delay(5);
		send_spi1[0]=0x52; //sector erase
		memcpy(&spi_send+1,&address_erase_b+1,3);   // copy 24bits
		HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 0);         // enable for sector erase   , stays empty when enabled
		HAL_SPI_Transmit(&hspi1, send_spi1, 4, 1000);   //erase sector ,works       4kbytes   (block erase=32kbytes or  64kbyte )
		HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, 1);
		HAL_Delay(100);  // this is slow


}
