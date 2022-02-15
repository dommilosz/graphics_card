u8 alloc_table[ALLOC_SECTORS];
u8 alloc_memory[ALLOC_SECTORS*SECTOR_SIZE];

void RemoveAsset(u8 asset){
    for(int i =0;i<ALLOC_SECTORS;i++){
        if(alloc_table[i] == asset){
            alloc_table[i] = 0;
        }
    }
}

u8 WriteAsset(u8 asset,u8 *data,u16 length){
    if(asset<1)return 0;
    RemoveAsset(asset);
    u16 wi = 0;
    for(u16 i =0;i<ALLOC_SECTORS;i++){
        if(alloc_table[i] == 0){
            alloc_table[i] = asset;
            memset(alloc_memory+(i*SECTOR_SIZE),0x00,SECTOR_SIZE);
            for(u8 w = 0;w<SECTOR_SIZE;w++){
                if(wi >= length)return 1;
                alloc_memory[(i*SECTOR_SIZE)+w] = data[wi];
                wi++;
                if(wi >= length)return 1;
            }
        }
    }
    return 0;
}

u8 WriteAssetFromDataSource(u8 asset,u16 length){
    if(asset<1)return 0;
    RemoveAsset(asset);
    u16 wi = 0;
    for(u16 i =0;i<ALLOC_SECTORS;i++){
        if(alloc_table[i] == 0){
            alloc_table[i] = asset;
            memset(alloc_memory+(i*SECTOR_SIZE),0x00,SECTOR_SIZE);
            for(u8 w = 0;w<SECTOR_SIZE;w++){
                if(wi >= length)return 1;
                alloc_memory[(i*SECTOR_SIZE)+w] = GetU8();
                wi++;
                if(wi >= length)return 1;
            }
        }
    }
    return 0;
}

u8 WriteAssetFromDataSource(u8 asset,u16 length,data_source source){
    UTIL_DATA_SOURCE = source;
    return WriteAssetFromDataSource(asset,length);
}

u8 ReadAsset(u8 asset, u16 byte){
    u16 sector = byte/SECTOR_SIZE;
    for(u16 i =0;i<ALLOC_SECTORS;i++){
        if(alloc_table[i] == asset){
            if(sector==0){
                return alloc_memory[(i*SECTOR_SIZE)+(byte%SECTOR_SIZE)];
            }
            sector--;
        }
    }
    return 0;
}

void GetSectors(u8 asset){
    for(u16 i =0;i<ALLOC_SECTORS;i++){
        if(alloc_table[i] == asset){
            printf(" %u ",i);
        }
    }
}
