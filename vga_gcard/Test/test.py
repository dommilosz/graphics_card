import serial
import time
import colorsys
import math

ser = serial.Serial('COM6')  # open serial port
print(ser.name)         # check which port was really used

def U16ToBytes(u16):
	return [u16 >> 8, u16 & 0x00FF]

def U32ToBytes(u32):
	return [u32 >> 24, u32 >> 16 & 0x00FF, u32 >> 8 & 0x0000FF, u32 & 0x000000FF]

def Fill(mode,offset,length,color):
	data = [5,mode]
	data += U32ToBytes(offset)
	data += U32ToBytes(length)
	data += [color]
	print(data)
	ser.write(data)
	print(ser.read());


def Rectangle(x1,y1,w,h,color):
	data = [6,0]
	data += U16ToBytes(x1)
	data += U16ToBytes(y1)
	data += U16ToBytes(w)
	data += U16ToBytes(h)
	data += [color]
	ser.write(data)
	print(ser.read());

def Line(x1,y1,x2,y2,color):
	data = [6,2]
	data += U16ToBytes(x1)
	data += U16ToBytes(y1)
	data += U16ToBytes(x2)
	data += U16ToBytes(y2)
	data += [color]
	ser.write(data)
	print(ser.read());

def Circle(x1,y1,r,color,mask):
	data = [6,3]
	data += U16ToBytes(x1)
	data += U16ToBytes(y1)
	data += U16ToBytes(r)
	data += [color,mask]
	ser.write(data)
	print(ser.read());

def CircleF(x1,y1,r,color,mask):
	data = [6,3|128]
	data += U16ToBytes(x1)
	data += U16ToBytes(y1)
	data += U16ToBytes(r)
	data += [color,mask]
	ser.write(data)
	print(ser.read());

def Clear(c):
	ser.write([3,c])

def Text(x,y,txt,color, fH = 8, sX = 1, sY = 1):
	#97 116 111 115 32 103 117 114 111 109 0
	data = [7,0]
	data += U16ToBytes(len(txt))
	data += U16ToBytes(x)
	data += U16ToBytes(y)
	data += [color,fH,sX,sY]
	data += bytes(txt,"ascii")
	ser.write(data)     # write a string
	print(ser.read());

def TextBg(x,y,txt,color,bgcolor, fH = 8, sX = 1, sY = 1):
	#97 116 111 115 32 103 117 114 111 109 0
	data = [7,0|64]
	data += U16ToBytes(len(txt))
	data += U16ToBytes(x)
	data += U16ToBytes(y)
	data += [color,bgcolor,fH,sX,sY]
	data += bytes(txt,"ascii")
	ser.write(data)     # write a string
	print(ser.read());

def TextC(x,y,txt,color, fH = 8, sX = 1, sY = 1):
	#97 116 111 115 32 103 117 114 111 109 0
	data = [7,1]
	data += U16ToBytes(len(txt))
	data += [color,fH,sX,sY]
	data += bytes(txt,"ascii")
	ser.write(data)     # write a string
	print(ser.read());

def TextBgC(x,y,txt,color,bgcolor, fH = 8, sX = 1, sY = 1):
	#97 116 111 115 32 103 117 114 111 109 0
	data = [7,1|64]
	data += U16ToBytes(len(txt))
	data += [color,bgcolor,fH,sX,sY]
	data += bytes(txt,"ascii")
	 # write a string
	print(ser.read());

def Point(color,x,y):
	data = [6,1]
	data += U16ToBytes(x)
	data += U16ToBytes(y)
	data += [color]
	ser.write(data)    
	print(ser.read());

def Points(color,points):
	data = [6,1|64]
	data += U16ToBytes(len(points))
	data += [color]
	for p in points:
		data += U16ToBytes(p[0])
		data += U16ToBytes(p[1])
	ser.write(data)    
	print(ser.read());

def Init(resolution):
	ser.write([4,resolution]);

def SetCursors(x,y):
	data = [7,128]
	data += U16ToBytes(x)
	data += U16ToBytes(y)
	ser.write(data)
	print(ser.read());

def transition():
	#RRR GGG BB
	for h in range(100):
		c = hsv28rgb(h/100,1,1)
		c2 = c[0]<<5|c[1]<<2|c[2]
		print(c)
		print(c2)
		Fill(128,0,0xFFFFFFFF,c2)
		time.sleep(0.05)

def hsv2rgb(h,s,v):
	return tuple(round(i * 255) for i in colorsys.hsv_to_rgb(h,s,v))

def hsv28rgb(h,s,v):
	rgb24 = hsv2rgb(h,s,v)
	return (math.floor(rgb24[0]/256*8),math.floor(rgb24[1]/256*8),math.floor(rgb24[2]/256*4))

def ImgStream():
	from PIL import Image
	im = Image.open( 'prundnot.png')
	pixels = list(im.getdata())
	width, height = im.size
	pixels = [pixels[i * width:(i + 1) * width] for i in range(height)]

	def to8bit(pixel):
		r=int(pixel[0]/256*8)
		g=int(pixel[1]/256*8)
		b=int(pixel[2]/256*8)
		print(pixel)
		return (r<<5)|(g<<2)|b
		# rrrgggbb
	
	for x in range(0,len(pixels)):
		for y in range(0,len(pixels[x])):
			pixel = pixels[x][y]
			if(len(pixel)<4 or pixel[3]>0):
				#print("x:{} y:{} {}".format(x,y,pixel))

				color = to8bit(pixel) 
				Point(color,y,x)
	
def Test_Objects():
	ser.write([8,1,0,1])
	print(ser.read());
	ser.write([8,1,5,0,0,200,0,200]);
	print(ser.read());
	ser.write([8,1,3,255]);
	print(ser.read());

	ser.write([8,3,0,1])
	print(ser.read());
	ser.write([8,3,5,0,0,200,0,200]);
	print(ser.read());
	ser.write([8,3,3,100]);
	print(ser.read());
	ser.write([8,3,2,0,0,100,0,100]);
	print(ser.read());

	ser.write([8,4,0,3])
	print(ser.read());
	ser.write([8,4,6,0,0,40]);
	print(ser.read());
	ser.write([8,4,3,69]);
	print(ser.read());
	ser.write([8,4,2,0,0,100,0,100]);
	print(ser.read());
	ser.write([8,4,7,255,0]);
	print(ser.read());

	for i in range(100):
		ser.write([8,4,2,1,0,1,0,1]);
		print(ser.read());
		ser.write([8,3,2,1,255,255,255,255]);
		print(ser.read());
		time.sleep(0.1)

def Sound(_data, repeat):
	if(len(_data)>65535):
		return
	data = [9]
	data += U16ToBytes(len(data))
	data += [repeat]
	data += _data;
	ser.write(data)
	print(ser.read());

def SoundS(_data, repeat):
	for sound in _data:
		Sound([sound],repeat)

def SoundS(_data, repeat):
	ser.write([8,1,0])
	print(ser.read())
	ser.write([8,1,0])
	print(ser.read())


#512*400
Init(2);
time.sleep(5)
CreateTextObj()
#transition()
#Clear(0)
#Test_Objects()
#Line(0,0,170,400,0b11100000)
#Line(170,0,340,400,0b00011100)
#Line(340,0,511,400,0b00000011)

#ImgStream()

#for i in range(256):
#	Sound([i,i],False)
#	#time.sleep(0.1)

ser.close()     
