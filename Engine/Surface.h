#pragma once
#include "WinStreamline.h"
#include "Colors.h"
#include "Rect.h"
#include "ChiliException.h"
#include <string>
#include <assert.h>
#include <memory>


#include <iostream>


class Surface
{
public:
	class Exception : public ChiliException
	{
	public:
		using ChiliException::ChiliException;
		virtual std::wstring GetFullMessage() const override { return GetNote() + L"\nAt: " + GetLocation(); }
		virtual std::wstring GetExceptionType() const override { return L"Surface Exception"; }
	};
public:
	Surface( unsigned int width,unsigned int height,unsigned int pitch )
		:
		pBuffer(std::make_unique<Color[]>(pitch* height)),
		/*pBuffer(new Color[pitch * height]),*/
		width( width ),
		height( height ),
		pitch( pitch )
	{
		// TEST
		pBufferDup = pBuffer.get();
	
	}
	Surface( unsigned int width,unsigned int height )
		:
		Surface( width,height,width )
	{
	}
	Surface( Surface&& source )
		:
		pBuffer( std::move( source.pBuffer ) ),
		width( source.width ),
		height( source.height ),
		pitch( source.pitch )
	{}
	Surface( Surface& ) = delete;
	Surface& operator=( Surface&& donor )
	{
		width = donor.width;
		height = donor.height;
		pitch = donor.pitch;
		pBuffer = std::move( donor.pBuffer );
		donor.pBuffer = nullptr;
		return *this;
	}
	Surface& operator=( const Surface& ) = delete;
	~Surface()
	{}
	void Clear( Color fillValue  )
	{
		//memset( pBuffer,fillValue.dword,pitch * height * sizeof( Color ) );
		memset(pBuffer.get(), fillValue.dword, pitch * height * sizeof(Color)); // unique_ptr
				
	}
	void Present( unsigned int dstPitch,BYTE* const pDst ) const
	{
		for( unsigned int y = 0; y < height; y++ )
		{
			memcpy( &pDst[dstPitch * y],&pBuffer[pitch * y],sizeof(Color) * width );
		}
	}
	void PutPixel( unsigned int x,unsigned int y,Color c )
	{
		assert( x >= 0 );
		assert( y >= 0 );
		assert( x < width );
		assert( y < height );
		pBuffer[y * pitch + x] = c;
	}
	void PutPixelAlpha( unsigned int x,unsigned int y,Color c );
	Color GetPixel( unsigned int x,unsigned int y ) //const
	{

		assert( x >= 0 );
		assert( y >= 0 );
		assert( x < width );
		assert( y < height );
		return pBuffer[y * pitch + x];
		//return pBufferDup[y * pitch + x];
		//pBufferDup = pBuffer.get();
		//return *(pBufferDup + y * pitch + x);
	}
	unsigned int GetWidth() const
	{
		return width;
	}
	unsigned int GetHeight() const
	{
		return height;
	}
	unsigned int GetPitch() const
	{
		return pitch;
	}
	Color* GetBufferPtr()
	{
		return pBuffer.get(); // unique_ptr
		//return pBuffer;
	}
	const Color* GetBufferPtrConst() const
	{
		return pBuffer.get(); // unique_ptr
		//return pBuffer;
	}
	const unsigned int* GetRGBApointerConst() const {
		return pRGBAbuffer.get();
	}

	void makeRGBA() {
		for (unsigned int y=0; y < height; y++)
			for (unsigned int x = 0; x < width; x++) {
				unsigned int ARGBcolor = pBuffer[y * pitch + x].dword;
				pBuffer[y * pitch + x].dword = (((ARGBcolor & 0xF0000000) >> 24) | ARGBcolor << 8);
				//((pBuffer[y * pitch + x].dword << 8) & 15);
			}

	}

	static Surface FromFile( const std::wstring& name );
	void Save( const std::wstring& filename ) const;
	void Copy( const Surface& src );
private:
	// calculate pixel pitch required for given byte aligment (must be multiple of 4 bytes)
	static unsigned int GetPitch( unsigned int width,unsigned int byteAlignment )
	{
		assert( byteAlignment % 4 == 0 );
		const unsigned int pixelAlignment = byteAlignment / sizeof( Color );
		return width + ( pixelAlignment - width % pixelAlignment ) % pixelAlignment;
	}
	Surface( unsigned int width,unsigned int height,unsigned int pitch,std::unique_ptr<Color[]> pBufferParam, std::unique_ptr<unsigned int[]> rgbaBuffer )
		:
		width( width ),
		height( height ),
		pBuffer(std::move(pBufferParam)),		
		pitch( pitch ),
		pRGBAbuffer(std::move(rgbaBuffer))
	{

		// TEST
		pBufferDup = pBuffer.get();
		

	}
private:
	std::unique_ptr<Color[]> pBuffer;
	std::unique_ptr<unsigned int[]> pRGBAbuffer;	
	Color* pBufferDup;
	unsigned int width;
	unsigned int height;
	unsigned int pitch; // pitch is in PIXELS, not bytes!
};