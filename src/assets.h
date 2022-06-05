struct asset{
    u8 *data;
    u16 length;
};

asset alloc_table[ALLOC_SECTORS];

void RemoveAsset(u8 asset)
{
    if (alloc_table[asset].data != NULL)
        free(alloc_table[asset].data);
    alloc_table[asset].data = 0;
}

bool RelocateAsset(u8 asset,u16 new_length){
    if(alloc_table[asset].data != NULL){
        u8 *new_data = (u8*)malloc(new_length);
        memcpy(new_data, alloc_table[asset].data,alloc_table[asset].length);
        alloc_table[asset].length = new_length;
        free(alloc_table[asset].data);
        alloc_table[asset].data = new_data;
        return true;
    }
    return false;
}

bool PrepareAsset(u8 asset, u16 length, u16 *offset){
    if(asset < 1)return false;
    if (*offset == 0xFFFF)
    {
        *offset = 0;
        RemoveAsset(asset);
        alloc_table[asset].data = (u8*)malloc(length);
        alloc_table[asset].length = length;
    }
    if(alloc_table[asset].data == NULL){
        alloc_table[asset].data = (u8*)malloc(length+*offset);
        alloc_table[asset].length = length+*offset;
        return true;
    }
    if((*offset+length) <= alloc_table[asset].length){
        return true;
    }
    if(*offset > 0){
        RelocateAsset(asset,length+*offset);
    }
    if((*offset+length) > alloc_table[asset].length){
        RelocateAsset(asset,length+*offset);
    }
    return true;
}

u8 WriteAsset(u8 asset, u8 *data, u16 length, u16 offset = 0xFFFF)
{
    if(!PrepareAsset(asset,length,&offset))return 0;
    memcpy(alloc_table[asset].data+offset,data,length);
    return 1;
}

u8 WriteAssetFromProvider(u8 asset, u16 length, u8Func provider, u16 offset = 0xFFFF)
{
    if(!PrepareAsset(asset,length,&offset))return 0;
    for(int i =0;i<length;i++){
        alloc_table[asset].data[offset+i] = provider();
    }
    return 1;
}

u8 WriteAssetFromDataSource(u8 asset, u16 length, u16 offset = 0xFFFF)
{
    return WriteAssetFromProvider(asset, length, WaitForByte, offset);
}

u8 WriteAssetFromDataSource(u8 asset, u16 length, data_source source)
{
    UTIL_DATA_SOURCE = source;
    return WriteAssetFromDataSource(asset, length);
}

u8 ReadAsset(u8 asset, u16 byte)
{
    return *(alloc_table[asset].data+byte);
}

u16 Read2Asset(u8 asset, u16 byte)
{
    return ((ReadAsset(asset, byte) << 8) | ReadAsset(asset, byte + 1));
}

u32 Read3Asset(u8 asset, u16 byte)
{
    return ((ReadAsset(asset, byte) << 16) | (ReadAsset(asset, byte + 1) << 8) | ReadAsset(asset, byte + 2));
}

uint32_t AssetCRC32(u8 asset, u16 length)
{
    uint32_t crc = 0xFFFFFFFF;

    for (size_t i = 0; i < length; i++)
    {
        char ch = ReadAsset(asset, i);
        for (size_t j = 0; j < 8; j++)
        {
            uint32_t b = (ch ^ crc) & 1;
            crc >>= 1;
            if (b)
                crc = crc ^ 0xEDB88320;
            ch >>= 1;
        }
    }

    return ~crc;
}

uint32_t StrlenAsset(u8 asset)
{
    uint32_t l = 0;
    while (ReadAsset(asset, l) != 0)
    {
        l++;
    }
    return l;
}