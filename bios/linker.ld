OUTPUT_FORMAT(binary)

SECTIONS {
	. = 0x0000;
	.boot : { 
		*(.boot) 
	}
	. = 0xfff0;
	.reset : { 
		*(.reset)
		. = 0xf;
	}
}
