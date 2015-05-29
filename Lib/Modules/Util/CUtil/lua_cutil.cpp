/**
 * Lua module to expose some common c utilties
 *
 * University of Pennsylvania
 * 2010
 */

#include <lua.hpp>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <vector>
#include <string>
#include <map>

#ifdef TORCH
#include <torch/luaT.h>
#ifdef __cplusplus
extern "C"
{
#endif
#include <torch/TH/TH.h>
#ifdef __cplusplus
}
#endif
#endif


const char ascii_lut[] = "0123456789abcdef";
const int8_t byte_lut[] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                            0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                            0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                            0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                            0x0, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                            0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                            0x0, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                            0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};




//color.robotblue = 32;
//color.robotpink = 64;
//Pack 32 possible label color bits into 6 types (for monitoring)
//Priority: Orange > Yellow > Cyan > White > Green > Black
//Black:0, Orange:1, Yellow:2, RobotBlue: 4, Green:8, White: 16, RobotPink: 32
//Map : 0,      1,       2,           3,         4,        5 ,          6,             

const int8_t label_color_pack_lut[]=
	{ 0, 1, 
	        2, 1, 
                 3, 1, 2, 1, 
                             4, 1, 2, 1, 3, 1, 2, 1, 
                                                     5, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1,

    6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1,   6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1,   

        };

const int8_t label_color_unpack_lut[]={0,1,2,4,8,16,32};

const char label_lut1[] = "0123456";
const char label_lut2[] = "abcdefg";


std::map<std::string, int> dataTypeMap;

// use matlab support datatype names
void init_dataTypeMap() {
  dataTypeMap["int8"]     = sizeof(int8_t);
  dataTypeMap["int16"]    = sizeof(int16_t);
  dataTypeMap["int32"]    = sizeof(int32_t);
  dataTypeMap["int64"]    = sizeof(int64_t);
  dataTypeMap["uint8"]    = sizeof(uint8_t);
  dataTypeMap["uint16"]   = sizeof(uint16_t);
  dataTypeMap["uint32"]   = sizeof(uint32_t);
  dataTypeMap["uint64"]   = sizeof(uint64_t);
  dataTypeMap["single"]   = sizeof(float);
  dataTypeMap["double"]   = sizeof(double);
}


static int lua_array2string(lua_State *L) {
  uint8_t *data = (uint8_t *) lua_touserdata(L, 1);
  if ((data == NULL) || !lua_islightuserdata(L, 1)) {
    return luaL_error(L, "Input image not light user data");
  }
  
  int width = luaL_checkint(L, 2);
  int height = luaL_checkint(L, 3);
  std::string dtype(luaL_checkstring(L, 4));
  std::string name(luaL_checkstring(L, 5));

  std::map<std::string, int>::iterator idataTypeMap = dataTypeMap.find(dtype);
  if (idataTypeMap == dataTypeMap.end()) {
    return luaL_error(L, "unkown dtype: %s", dtype.c_str());
  }
  int nbytes = idataTypeMap->second;

  int size = width*height * nbytes;
  char cdata[(2*size) + 1];

  int ind = 0;
  int cind = 0;
  while (ind < size) {    
    cdata[cind] = ascii_lut[(data[ind] & 0xf0) >> 4];
    cdata[cind+1] = ascii_lut[(data[ind] & 0x0f)];
    ind += 1;
    cind += 2;
  }
  cdata[(2*size)] = '\0';

  // create lua table
  lua_createtable(L, 0, 5);

  lua_pushstring(L, "name");
  lua_pushstring(L, name.c_str());
  lua_settable(L, -3);

  lua_pushstring(L, "width");
  lua_pushnumber(L, width);
  lua_settable(L, -3);

  lua_pushstring(L, "height");
  lua_pushnumber(L, height);
  lua_settable(L, -3);

  lua_pushstring(L, "dtype");

  lua_createtable(L, 0, 2);
  lua_pushstring(L, "name");
  lua_pushstring(L, dtype.c_str());
  lua_settable(L, -3);

  lua_pushstring(L, "nbytes");
  lua_pushnumber(L, nbytes);
  lua_settable(L, -3);

  lua_settable(L, -3);

  lua_pushstring(L, "data");
  lua_pushstring(L, cdata);
  lua_settable(L, -3);

  return 1;
}

static int lua_string2userdata(lua_State *L) {
  uint8_t *dout = (uint8_t *) lua_touserdata(L, 1);
  if ((dout == NULL) || !lua_islightuserdata(L, 1)) {
    return luaL_error(L, "output argument not light user data");
  }

  const char *cdata = luaL_checkstring(L, 2);

  int ind = 0;
  int cind = 0;
  while (cdata[cind] != '\0' && cdata[cind+1] != '\0') {
    uint8_t bh = cdata[cind] >= 'a' ? cdata[cind] - 'a' + 10 : cdata[cind] - '0';
    uint8_t bl = cdata[cind+1] >= 'a' ? cdata[cind+1] - 'a' + 10 : cdata[cind+1] - '0';
    dout[ind] = (uint8_t)((bh<<4) | bl);

    ind += 1;
    cind += 2;
  }

  return 1;
}




//Custom function for YUYV, where we throw out every other line

static int lua_array2string2(lua_State *L) {
  uint8_t *data = (uint8_t *) lua_touserdata(L, 1);
  if ((data == NULL) || !lua_islightuserdata(L, 1)) {
    return luaL_error(L, "Input image not light user data");
  }
  
  int width = luaL_checkint(L, 2);
  int height = luaL_checkint(L, 3);
  std::string dtype(luaL_checkstring(L, 4));
  std::string name(luaL_checkstring(L, 5));

  std::map<std::string, int>::iterator idataTypeMap = dataTypeMap.find(dtype);
  if (idataTypeMap == dataTypeMap.end()) {
    return luaL_error(L, "unkown dtype: %s", dtype.c_str());
  }
  int nbytes = idataTypeMap->second;
  int size = width*height * nbytes / 2;//half the size
  char cdata[(2*size) + 1];

  int ind = 0;
  int cind = 0;

  for (int i=0;i<height/2;i++){
    for (int j=0;j<width*nbytes;j++){
      cdata[cind] = ascii_lut[(data[ind] & 0xf0) >> 4];
      cdata[cind+1] = ascii_lut[(data[ind] & 0x0f)];
      ind += 1;
      cind += 2;
    }
    ind += width*nbytes;//skip every other line
  }

  cdata[(2*size)] = '\0';

  // create lua table
  lua_createtable(L, 0, 5);

  lua_pushstring(L, "name");
  lua_pushstring(L, name.c_str());
  lua_settable(L, -3);

  lua_pushstring(L, "width");
  lua_pushnumber(L, width);
  lua_settable(L, -3);

  lua_pushstring(L, "height");
  lua_pushnumber(L, height);
  lua_settable(L, -3);

  lua_pushstring(L, "dtype");

  lua_createtable(L, 0, 2);
  lua_pushstring(L, "name");
  lua_pushstring(L, dtype.c_str());
  lua_settable(L, -3);

  lua_pushstring(L, "nbytes");
  lua_pushnumber(L, nbytes);
  lua_settable(L, -3);

  lua_settable(L, -3);

  lua_pushstring(L, "data");
  lua_pushstring(L, cdata);
  lua_settable(L, -3);

  return 1;
}

//function for halved yuyv data string
static int lua_string2userdata2(lua_State *L) {
  uint8_t *dout = (uint8_t *) lua_touserdata(L, 1);
  if ((dout == NULL) || !lua_islightuserdata(L, 1)) {
    return luaL_error(L, "output argument not light user data");
  }

  const char *cdata = luaL_checkstring(L, 2);
  int width = luaL_checkint(L, 3);
  int height = luaL_checkint(L, 4);

  int ind = 0;
  int cind = 0;
  int nbyte = 4; //this function is only used for yuyv

  for (int i=0;i<height/2;i++){
    for (int j=0;j<width*nbyte;j++){
      uint8_t bh = cdata[cind] >= 'a' ? 
		cdata[cind] - 'a' + 10 : cdata[cind] - '0';
      uint8_t bl = cdata[cind+1] >= 'a' ? 
		cdata[cind+1] - 'a' + 10 : cdata[cind+1] - '0';
      dout[ind] = (uint8_t)((bh<<4) | bl);
      ind += 1;
      cind += 2;
    }
    //Copy previous line
    for (int j=0;j<width*nbyte;j++){
      dout[ind] = dout[ind-width*nbyte];
      ind += 1;
    }
  }
  return 1;
}


//Label-specific string conversion function
//Bin each label into 6 class 
//And run Run-length encoding

static int lua_label2string_rle(lua_State *L) {
  uint8_t *data = (uint8_t *) lua_touserdata(L, 1);
  if ((data == NULL) || !lua_islightuserdata(L, 1)) {
    return luaL_error(L, "Input image not light user data");
  }
  
  int arr_size = luaL_checkint(L, 2);
  std::string dtype(luaL_checkstring(L, 3));
  std::string name(luaL_checkstring(L, 4));

  std::map<std::string, int>::iterator idataTypeMap = dataTypeMap.find(dtype);
  if (idataTypeMap == dataTypeMap.end()) {
    return luaL_error(L, "unkown dtype: %s", dtype.c_str());
  }
  int nbytes = idataTypeMap->second;

  int size = arr_size * nbytes;
  char cdata[size + 1];

  int ind = 0;
  int cind = 0;
  
  int last_data=label_color_pack_lut[data[0]];
  int current_size=1;
  int total_byte = 0;
  ind++;

  while (ind < size) {    
    int current_data = label_color_pack_lut[data[ind]];    
    if (ind==size-1) {
      //Multiple data : "012345"
      cdata[cind++] = '0' + last_data;
      cdata[cind++] = '0' + current_size;
      total_byte+=2;
      ind++;
    }else{
      if ((current_data==last_data) && (current_size<75)){
        current_size++;
        ind++;
      }else{
//	printf("C%dS%d, ",last_data,current_size);
	if (current_size>1){
          //Multiple data : "0123456"
          cdata[cind++] = '0' + last_data;
          cdata[cind++] = '0' + current_size;
          total_byte+=2;
	}else{
          //Single data : "789:; "
          cdata[cind++] = '7' + last_data;
          total_byte+=1;
	}
        last_data = current_data;
        current_size = 1;
        ind++;
      }
    }
  }
  cdata[cind] = '\0';


  // create lua table
  lua_createtable(L, 0, 5);

  lua_pushstring(L, "name");
  lua_pushstring(L, name.c_str());
  lua_settable(L, -3);

  lua_pushstring(L, "size");
  lua_pushnumber(L, arr_size);
  lua_settable(L, -3);

  lua_pushstring(L, "bsize");
  lua_pushnumber(L, total_byte);
  lua_settable(L, -3);

  lua_pushstring(L, "dtype");

  lua_createtable(L, 0, 2);
  lua_pushstring(L, "name");
  lua_pushstring(L, dtype.c_str());
  lua_settable(L, -3);


  lua_pushstring(L, "nbytes");
  lua_pushnumber(L, nbytes);
  lua_settable(L, -3);

  lua_settable(L, -3);

  lua_pushstring(L, "data");
  lua_pushstring(L, cdata);
  lua_settable(L, -3);

  return 1;
}



static int lua_string2label_rle(lua_State *L) {
  uint8_t *dout = (uint8_t *) lua_touserdata(L, 1);
  if ((dout == NULL) || !lua_islightuserdata(L, 1)) {
    return luaL_error(L, "output argument not light user data");
  }
  const char *cdata = luaL_checkstring(L, 2);
  int ind = 0;
  int cind = 0;
  while (cdata[cind] != '\0' && cdata[cind+1] != '\0') {
    int data1 = cdata[cind++]-'0';
    if (data1>6) {
      //Single data
      dout[ind++] = label_color_unpack_lut[data1-6];
    }else{
      //Multiple data
      int len = cdata[cind++]-'0';
      for (int i=0;i<len;i++){
	dout[ind++]=label_color_unpack_lut[data1];
      }
    }
  }
  return 1;
}

static int lua_testlabel(lua_State *L) {
  int size = luaL_checkint(L, 1);  
  int mod = luaL_checkint(L, 2);  
  static uint8_t *ptr = NULL;  
  if (ptr == NULL) {
    ptr = (uint8_t*)malloc(size*sizeof(uint8_t));
    for (int i = 0; i < size; i++) ptr[i] = pow(2,((i+mod)%4));
  }
  lua_pushlightuserdata(L, ptr);
  return 1;
}



static int lua_label2array_rle(lua_State *L) {
  uint8_t *data = (uint8_t *) lua_touserdata(L, 1);
  if ((data == NULL) || !lua_islightuserdata(L, 1)) {
    return luaL_error(L, "Input image not light user data");
  }
  int arr_size = luaL_checkint(L, 2);
  int size = arr_size;
  uint8_t cdata[size + 1];
  int ind = 0;
  int cind = 0;
  int last_data=label_color_pack_lut[data[0]];
  int current_size=1;
  int total_byte = 0;
  ind++;
//One char: 0-255
//data: low 3 bit for data (0-5), high 5 bit for repetition (0~31) 
  while (ind < size) {    
    int current_data = label_color_pack_lut[data[ind]];
    if (ind==size-1) {
      cdata[cind++] = last_data + 8*(current_size-1);
      cdata[cind++] = current_data;
      total_byte+=1;
      ind++;
    }else{
      if ((current_data==last_data) && (current_size<32)){
        current_size++;
        ind++;
      }else{
        cdata[cind++] = last_data + 8*(current_size-1);
        total_byte+=1;
        last_data = current_data;
        current_size = 1;
        ind++;
      }
    }
  }
  lua_createtable(L, cind, 0);
  for (int i=0; i<cind; i++) {
    lua_pushnumber(L, cdata[i]);
    lua_rawseti(L, -2, i+1);
  }
  return 1;
}


static int lua_array2label_rle(lua_State *L) {
  uint8_t *dout = (uint8_t *) lua_touserdata(L, 1);
  if ((dout == NULL) || !lua_islightuserdata(L, 1)) {
    return luaL_error(L, "output argument not light user data");
  }
  int num = lua_tonumber(L, 2);
  lua_remove(L,1);
  lua_remove(L,1);

  int ind = 0;

  for (int i=0; i<num; i++) {
    lua_pushinteger(L, i+1);
    lua_gettable(L, -2);

    int data_byte = (int) lua_tonumber(L, -1);
    int len = data_byte/8 + 1;
    int data = label_color_unpack_lut[data_byte%8];
    for (int j=0;j<len;j++) dout[ind++]=data;
    lua_pop(L, 1);
  }
  return 1;

}

static int lua_printlabel(lua_State *L) {
  uint8_t *data = (uint8_t *) lua_touserdata(L, 1);
  if ((data == NULL) || !lua_islightuserdata(L, 1)) {
    return luaL_error(L, "Input image not light user data");
  }
  int num = lua_tonumber(L, 2);
  for (int i=0;i<num;i++){
    printf("%d ",data[i]);

  }
  printf("\n");
  return 1;
}

static int lua_ptradd(lua_State *L) {
  uint8_t *ptr = (uint8_t *) lua_touserdata(L, 1);
  if ((ptr == NULL) || !lua_islightuserdata(L, 1)) {
    return luaL_error(L, "ptr argument not light user data");
  }

  int n = luaL_checkinteger(L, 2);
  std::string dtype(luaL_checkstring(L, 3));

  std::map<std::string, int>::iterator idataTypeMap = dataTypeMap.find(dtype);
  if (idataTypeMap == dataTypeMap.end()) {
    return luaL_error(L, "unkown dtype: %s", dtype.c_str());
  }
  int nbytes = n * (idataTypeMap->second);

  lua_pushlightuserdata(L, ptr + nbytes); 

  return 1;
}

static int lua_sizeof(lua_State *L) {
  std::string dtype(luaL_checkstring(L, 1));

  std::map<std::string, int>::iterator idataTypeMap = dataTypeMap.find(dtype);
  if (idataTypeMap == dataTypeMap.end()) {
    return luaL_error(L, "unkown dtype: %s", dtype.c_str());
  }
  int nbytes = idataTypeMap->second;
  lua_pushinteger(L, nbytes); 
  return 1;
}

static int lua_testarray(lua_State *L) {
  static uint32_t *ptr = NULL;
  int size = 640*480;
  if (ptr == NULL) {
    ptr = (uint32_t*)malloc(size*sizeof(uint32_t));
    for (int i = 0; i < size; i++) ptr[i] = i;
  }
  lua_pushlightuserdata(L, ptr);
  return 1;
}

static int lua_bitand(lua_State *L) {
  int a = luaL_checkint(L, 1); 
  int b = luaL_checkint(L, 2); 
  lua_pushinteger(L, a & b);
  return 1;
}


static int lua_bitor(lua_State *L) {
  int a = luaL_checkint(L, 1); 
  int b = luaL_checkint(L, 2); 

  lua_pushinteger(L, a | b);

  return 1;
}

static int lua_bitxor(lua_State *L) {
  int a = luaL_checkint(L, 1); 
  int b = luaL_checkint(L, 2); 

  lua_pushinteger(L, a ^ b);

  return 1;
}

static int lua_bitnot(lua_State *L) {
  int a = luaL_checkint(L, 1); 

  lua_pushinteger(L, ~a);

  return 1;
}

#ifdef TORCH
static int lua_torch_to_userdata (lua_State *L) {
  /* Grab the destination tensor */
  const char* t_name = luaT_typename(L,1);
  THByteTensor *b_t = (THByteTensor *)luaT_checkudata(L, 1, t_name);
  lua_pushlightuserdata(L,b_t->storage->data);
  return 1;
}
#endif

static const luaL_Reg cutil_lib [] = {
  {"array2string", lua_array2string},
  {"string2userdata", lua_string2userdata},
  {"array2string2", lua_array2string2},
  {"string2userdata2", lua_string2userdata2},
  


  {"label2string_rle", lua_label2string_rle},
  {"string2label_rle", lua_string2label_rle},
  {"label2array_rle", lua_label2array_rle},
  {"array2label_rle", lua_array2label_rle},
  {"ptr_add", lua_ptradd},
  {"bit_and", lua_bitand},
  {"bit_or", lua_bitor},
  {"bit_xor", lua_bitxor},
  {"bit_not", lua_bitnot},
  {"sizeof", lua_sizeof},
  {"test_array", lua_testarray},
  {"test_label", lua_testlabel},
  {"print_label", lua_printlabel},
#ifdef TORCH
  {"torch_to_userdata", lua_torch_to_userdata},
#endif

  {NULL, NULL}
};

#ifdef __cplusplus
extern "C"
#endif
int luaopen_cutil (lua_State *L) {
#if LUA_VERSION_NUM == 502
	luaL_newlib(L, cutil_lib);
#else
  luaL_register(L, "cutil", cutil_lib);
#endif
  init_dataTypeMap();
  
  return 1;
}
