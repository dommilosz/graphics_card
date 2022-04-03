u8 alloc_table[ALLOC_SECTORS];
u8 alloc_memory[ALLOC_SECTORS * SECTOR_SIZE];

void RemoveAsset(u8 asset)
{
    for (int i = 0; i < ALLOC_SECTORS; i++)
    {
        if (alloc_table[i] == asset)
        {
            alloc_table[i] = 0;
        }
    }
}

u8 WriteAsset(u8 asset, u8 *data, u16 length)
{
    if (asset < 1)
        return 0;
    RemoveAsset(asset);
    u16 wi = 0;
    for (u16 i = 0; i < ALLOC_SECTORS; i++)
    {
        if (alloc_table[i] == 0)
        {
            alloc_table[i] = asset;
            memset(alloc_memory + (i * SECTOR_SIZE), 0x00, SECTOR_SIZE);
            for (u8 w = 0; w < SECTOR_SIZE; w++)
            {
                if (wi >= length)
                    return 1;
                alloc_memory[(i * SECTOR_SIZE) + w] = data[wi];
                wi++;
                if (wi >= length)
                    return 1;
            }
        }
    }
    return 0;
}

u8 WriteAssetFromDataSource(u8 asset, u16 length)
{
    if (asset < 1)
        return 0;
    RemoveAsset(asset);
    u16 wi = 0;
    for (u16 i = 0; i < ALLOC_SECTORS; i++)
    {
        if (alloc_table[i] == 0)
        {
            alloc_table[i] = asset;
            memset(alloc_memory + (i * SECTOR_SIZE), 0x00, SECTOR_SIZE);
            for (u8 w = 0; w < SECTOR_SIZE; w++)
            {
                if (wi >= length)
                    return 1;
                alloc_memory[(i * SECTOR_SIZE) + w] = WaitForByte();
                wi++;
                if (wi >= length)
                    return 1;
            }
        }
    }
    return 0;
}

u8 WriteAssetFromDataSource(u8 asset, u16 length, u16 offset)
{
    if (asset < 1)
        return 0;
    u16 wi = 0;
    for (u16 i = 0; i < ALLOC_SECTORS; i++)
    {
        if (alloc_table[i] == 0 || alloc_table[i] == asset)
        {
            if (alloc_table[i] == 0)
            {
                memset(alloc_memory + (i * SECTOR_SIZE), 0x00, SECTOR_SIZE);
            }
            alloc_table[i] = asset;
            for (u8 w = 0; w < SECTOR_SIZE; w++)
            {
                if (offset > 0)
                {
                    offset--;
                    continue;
                }
                if (wi >= length)
                    return 1;
                alloc_memory[(i * SECTOR_SIZE) + w] = WaitForByte();
                wi++;
                if (wi >= length)
                    return 1;
            }
        }
    }
    return 0;
}

u8 WriteAssetFromDataSource(u8 asset, u16 length, data_source source)
{
    UTIL_DATA_SOURCE = source;
    return WriteAssetFromDataSource(asset, length);
}

u8 AssetSectorSize(u8 asset)
{
    u8 size = 0;
    for (u16 i = 0; i < ALLOC_SECTORS; i++)
    {
        if (alloc_table[i] == asset)
        {
            size += 1;
        }
    }
    return size;
}

u8 ReadAsset(u8 asset, u16 byte)
{
    u16 sector = byte / SECTOR_SIZE;
    for (u16 i = 0; i < ALLOC_SECTORS; i++)
    {
        if (alloc_table[i] == asset)
        {
            if (sector == 0)
            {
                return alloc_memory[(i * SECTOR_SIZE) + (byte % SECTOR_SIZE)];
            }
            sector--;
        }
    }
    return 0;
}

u16 Read2Asset(u8 asset, u16 byte)
{
    return ((ReadAsset(asset, byte) << 8) | ReadAsset(asset, byte + 1));
}

u32 Read3Asset(u8 asset, u16 byte)
{
    return ((ReadAsset(asset, byte) << 16) | (ReadAsset(asset, byte+1) << 8) | ReadAsset(asset, byte + 2));
}

void GetSectors(u8 asset)
{
    for (u16 i = 0; i < ALLOC_SECTORS; i++)
    {
        if (alloc_table[i] == asset)
        {
            printf(" %u ", i);
        }
    }
}


uint32_t AssetCRC32(u8 asset, u16 length) {
	uint32_t crc=0xFFFFFFFF;

	for(size_t i=0;i<length;i++) {
		char ch = ReadAsset(asset,i);
		for(size_t j=0;j<8;j++) {
			uint32_t b=(ch^crc)&1;
			crc>>=1;
			if(b) crc=crc^0xEDB88320;
			ch>>=1;
		}
	}
	
	return ~crc;
}

uint32_t StrlenAsset(u8 asset){
    uint32_t l = 0;
    while(ReadAsset(asset, l) != 0){
        l++;
    }
    return l;
}