#pragma once

#include "../libSystem/piTypes.h"

namespace piLibs {


typedef void * piTexture;
typedef void * piShader;
typedef void * piVertexArray;
typedef void * piFont3D;
typedef void * piRTarget;
typedef void * piSampler;
typedef void * piBuffer;
typedef void * piRasterState;
typedef void * piBlendState;
typedef void * piDepthState;



typedef enum
{
    piFORMAT_C1I8 =  0,
    piFORMAT_C2I8 =  1,
    piFORMAT_C3I8_RGB =  2,
    piFORMAT_C3I8_BGR =  3,
    piFORMAT_C4I8_BGRA =  4,
    piFORMAT_C4I8_RGBA =  5,
    piFORMAT_C4I8_RGBA_ENDIAN =  47,

    piFORMAT_C1F16 =  6,
    piFORMAT_C2F16 =  7,
    piFORMAT_C3F16_RGB =  8,
    piFORMAT_C4F16_RGBA =  9,
    piFORMAT_C3F16_BGR =  10,
    piFORMAT_C4F16_BGRA =  11,

    piFORMAT_C1F32 =  12,
    piFORMAT_C2F32 = 13,
    piFORMAT_C3F32_RGB = 14,
    piFORMAT_C4F32_RGBA = 15,

    piFORMAT_C1I8I = 16,
    piFORMAT_C1I16I = 17,
    piFORMAT_C1I32I = 18,

    piFORMAT_D24 = 40,
	piFORMAT_D32 = 41,
    piFORMAT_D32F = 42,

    piFORMAT_C4I1010102_BGRA = 43,
    piFORMAT_C4I1010102_RGBA = 44,
    piFORMAT_C3I111110_BGRA = 45,
    piFORMAT_C3I111110_RGBA = 46,

    piFORMAT_UNKOWN = 100
}piTextureFormat;

typedef enum
{
    piRArrayType_UByte    = 0,
    piRArrayType_Float    = 1,
    piRArrayType_Int      = 2,
    piRArrayType_Double   = 3,
    piRArrayType_Half     = 4,
}piRArrayDataType;

typedef struct
{
    int             mStride;
    int             mNumElements;
    int             mDivisor;
    struct
    {
        int              mNumComponents;
	    piRArrayDataType mType;
	    bool             mNormalize;
    }mEntry[12];
}piRArrayLayout;

typedef enum
{
    piBLEND_ONE  = 0,
    piBLEND_SRC_ALPHA = 1,
    piBLEND_SRC_COLOR = 2,
    piBLEND_ONE_MINUS_SRC_COLOR = 3,
    piBLEND_ONE_MINUS_SRC_ALPHA = 4,
    piBLEND_DST_ALPHA = 5,
    piBLEND_ONE_MINUS_DST_ALPHA = 6,
    piBLEND_DST_COLOR = 7,
    piBLEND_ONE_MINUS_DST_COLOR = 8,
    piBLEND_SRC_ALPHA_SATURATE = 9,
    piBLEND_ZERO = 10
}piBlendOperations;

typedef enum
{
    piBLEND_ADD  = 0,
    piBLEND_SUB = 1,
    piBLEND_RSUB = 2,
    piBLEND_MIN = 3,
    piBLEND_MAX = 4,
}piBlendEquation;

typedef enum
{
    piSTATE_CULL_FACE  = 0,
    piSTATE_DEPTH_TEST = 1,
	piSTATE_WIREFRAME = 2,
    piSTATE_FRONT_FACE = 3,
    piSTATE_BLEND = 4,
    piSTATE_ALPHA_TO_COVERAGE = 5,
    piSTATE_DEPTH_CLAMP = 6,
    piSTATE_VIEWPORT_FLIPY = 7
}piState;

typedef struct
{
    int mNum;
    struct
    {
        char mName[64];
        int  mValue;
    }mOption[64];
}piShaderOptions;

typedef  struct 
{
    uint32_t  count;
    uint32_t  instanceCount;
    uint32_t  first;
    uint32_t  baseInstance;
}piDrawArraysIndirectCommand;

typedef  struct 
{
    uint32_t  count;
    uint32_t  instanceCount;
    uint32_t  firstIndex;
    uint32_t  baseVertex;
    uint32_t  baseInstance;
}piDrawElementsIndirectCommand;


class piRenderReporter
{
public:
    piRenderReporter() {}
    ~piRenderReporter() {}

    virtual void Info( const char *str ) = 0;
	//-----------
    virtual void Error( const char *str, int level ) = 0;
	//-----------
    virtual void Begin( uint64_t memLeaks, uint64_t memPeak, int texLeaks, int texPeak ) = 0;
    virtual void Texture( const wchar_t *key, uint64_t kb, piTextureFormat format, bool compressed, int xres, int yres, int zres ) = 0;
    virtual void End( void ) = 0;

};

class piRenderer
{
public:
	enum class API : int
	{
		GL = 0,
		DX = 1
	};

	enum class RendererFeature : int
	{
		VIEWPORT_ARRAY = 0,
		VERTEX_VIEWPORT = 1
	};

	enum class IndexArrayFormat : int
	{
		UINT_32 = 0,
		UINT_16 = 1,
	};

	enum class TextureType : int
	{
		T1D = 0,
		T2D = 1,
		T3D = 2,
		TCUBE = 3,
		T2D_ARRAY = 4,
		TCUBE_ARRAY = 5
	};

	enum class TextureFilter : int
	{
		NONE = 0,
		LINEAR = 1,
		MIPMAP = 2,
		PCF = 3,
		NONE_MIPMAP = 4,
	};

	enum class TextureWrap : int
	{
		CLAMP_TO_BORDER = 0,
		CLAMP = 1,
		REPEAT = 2,
		MIRROR_CLAMP = 3,
		MIRROR_REPEAT = 4,
	};

	enum class BufferType : int
	{
		Static = 0,
		Dynamic = 1,
	};

	enum class BufferUse : int
	{
		Vertex = 1,
		Index = 2,
		Constant = 4,
		ShaderResource = 8,
		Atomics = 16,
		DrawCommands = 32
	};

	enum class Format : int
	{
		C4_32_FLOAT = 0,
		C4_32_UINT = 1,
		C4_32_SINT = 2,
		C4_16_FLOAT = 3,
		C4_16_UNORM = 4,
		C4_16_UINT = 5,
		C4_16_SNORM = 6,
		C4_16_SINT = 7,
		C4_8_UNORM = 8,
		C4_8_UNORM_SRGB = 9,
		C4_8_UINT = 10,
		C4_8_SNORM = 11,
		C4_8_SINT = 12,

		C3_32_FLOAT = 13,
		C3_32_UINT = 14,
		C3_32_SINT = 15,

		C2_32_FLOAT = 16,
		C2_32_UINT = 17,
		C2_32_SINT = 18,
		C2_16_FLOAT = 19,
		C2_16_UNORM = 20,
		C2_16_UINT = 21,
		C2_16_SNORM = 22,
		C2_16_SINT = 23,
		C2_8_UNORM = 24,
		C2_8_UINT = 25,
		C2_8_SNORM = 26,
		C2_8_SINT = 27,

		C1_32_FLOAT = 28,
		C1_32_UINT = 29,
		C1_32_SINT = 30,
		C1_16_FLOAT = 31,
		C1_16_UNORM = 32,
		C1_16_UINT = 33,
		C1_16_SNORM = 34,
		C1_16_SINT = 35,
		C1_8_UNORM = 36,
		C1_8_UINT = 37,
		C1_8_SNORM = 38,
		C1_8_SINT = 39,

		C4_10_10_10_2_UNORM = 40,
		C4_10_10_10_2_UINT = 41,
		C3_11_11_10_FLOAT = 42,
		C3_9_9_9_4_EXP = 43,

		D1_32_FLOAT = 44,
		D1_16_UNORM = 45,
		DS_32_8_UINT = 46,
		DS_24_8_UINT = 47
	};

	enum class BarrierType : int
	{
		SHADER_STORAGE = 1,
		UNIFORM = 2,
		ATOMICS = 4,
		IMAGE = 8,
		COMMAND = 16,
		TEXTURE = 32,
		ALL = 0
	};

	enum class PrimitiveType : int
	{
		Triangle = 0,
		Point = 1,
		TriangleStrip = 2,
		LineStrip = 3,
		TriPatch = 4,
		QuadPatch = 5,
		LinesAdj = 6,
		LineStripAdj = 7,
		Patch16 = 8,
		Patch32 = 9,
		Lines = 10
	};

	typedef struct
	{
		TextureType     mType;
		piTextureFormat mFormat;
		bool            mCompressed;
		int             mXres;
		int             mYres;
		int             mZres;
		int             mMultisample;
		unsigned int mDeleteMe;
	}TextureInfo;

	typedef struct
	{
		int mNumElements;
		struct
		{
			char mName[64];
			Format mFormat;
			bool   mPerInstance; // if not, it's per vertex
		}mEntry[12];
	}ArrayLayout2;

public:
	static piRenderer *Create( const API type );

protected:
    piRenderer();
public:
	~piRenderer();

    virtual bool      Initialize(int id, const void **hwnd, int num, bool disableVSync, bool disableErrors, piRenderReporter *reporter, void *device) = 0;
    virtual void      Deinitialize( void ) = 0;
	virtual bool      SupportsFeature(RendererFeature feature) = 0;
	virtual API       GetAPI(void) = 0;
    virtual void      Report( void ) = 0;
	// GL
    virtual void      SetActiveWindow( int id ) = 0;
    virtual void      Enable(void) = 0;
    virtual void      Disable(void) = 0;
    virtual void	  SwapBuffers(void) = 0;
	// DX
	virtual void *    GetContext(void) = 0;


    //--- render targets ---
    virtual piRTarget CreateRenderTarget(piTexture vtex0, piTexture vtex1, piTexture vtex2, piTexture vtex3, piTexture zbuf) = 0;
    virtual void      DestroyRenderTarget( piRTarget obj ) = 0;
    virtual bool      SetRenderTarget( piRTarget obj ) = 0;
    virtual void      RenderTargetSampleLocations(piRTarget vdst, const float *locations) = 0;
    virtual void      BlitRenderTarget( piRTarget dst, piRTarget src, bool color, bool depth ) = 0;
    virtual void      SetWriteMask( bool c0, bool c1, bool c2, bool c3, bool z ) = 0;
    virtual void      SetShadingSamples( int shadingSamples ) = 0;
    virtual void      RenderTargetGetDefaultSampleLocation(piRTarget vdst, const int id, float *location) = 0;

    //--- general ---
    virtual void      Clear( const float *color0, const float *color1, const float *color2, const float *color3, const bool depth0 ) = 0;
    virtual void      SetState( piState state, bool value ) = 0;
    virtual void      SetBlending(int buf, piBlendEquation equRGB, piBlendOperations srcRGB, piBlendOperations dstRGB,
                                           piBlendEquation equALP, piBlendOperations srcALP, piBlendOperations dstALP) = 0;
    virtual void      SetViewport(int id, const int *vp) = 0;
	virtual void      SetViewports(int num, const float *viewports) = 0; // x,y,w,h,zmin,zmax // 0.0, 0.0, 800.0, 600.0, 0.0, 1.0
	virtual void      GetViewports(int *num, float *viewports) = 0; // x,y,w,h,zmin,zmax

	virtual piRasterState CreateRasterState(bool wireframe, bool cullFace, bool frontFace, bool depthClamp, bool multiSample) = 0;
	virtual void SetRasterState(const piRasterState vme) = 0;
	virtual void DestroyRasterState( piRasterState vme) = 0;

	virtual piBlendState CreateBlendState(bool alphaToCoverage, bool enabled0) = 0;
	virtual void SetBlendState(const piRasterState vme) = 0;
	virtual void DestroyBlendState(piRasterState vme) = 0;

	virtual piDepthState CreateDepthState(bool alphaToCoverage, bool lessEqual) = 0;
	virtual void SetDepthState(const piRasterState vme) = 0;
	virtual void DestroyDepthState(piRasterState vme) = 0;


    //--- textures ---
    virtual piTexture CreateTexture( const wchar_t *key, const TextureInfo *info, TextureFilter filter, TextureWrap wrap, float aniso, void *buffer ) = 0;
    virtual void      DestroyTexture( piTexture obj ) = 0;
    virtual void      ClearTexture( piTexture vme, int level, const void *data ) = 0;
    virtual void      UpdateTexture( piTexture me, int x0, int y0, int z0, int xres, int yres, int zres, const void *buffer ) = 0;
	virtual void	  GetTextureRes( piTexture me, int *res ) = 0;
    virtual void      GetTextureFormat( piTexture me, piTextureFormat *format ) = 0;
    virtual void      GetTextureContent( piTexture me, void *data, const piTextureFormat fmt ) = 0;
    virtual void      GetTextureContent(piTexture vme, void *data, int x, int y, int z, int xres, int yres, int zres) = 0;
    virtual void      GetTextureInfo( piTexture me, TextureInfo *info ) = 0;
    virtual void      GetTextureSampling(piTexture vme, TextureFilter *rfilter, TextureWrap *rwrap) = 0;
    virtual void      ComputeMipmaps( piTexture me ) = 0;
    virtual void      AttachTextures( int num, piTexture vt0, piTexture vt1=0, piTexture vt2=0, piTexture vt3=0, piTexture vt4=0, piTexture vt5=0, piTexture vt6=0, piTexture vt7=0, piTexture vt8=0, piTexture vt9=0, piTexture vt10=0, piTexture vt11=0, piTexture vt12=0, piTexture vt13=0, piTexture vt14=0, piTexture vt15=0 ) = 0;
    virtual void      DettachTextures( void ) = 0;
    virtual piTexture CreateTextureFromID(unsigned int id, TextureFilter filter) = 0;
    virtual void      MakeResident( piTexture vme ) = 0;
    virtual void      MakeNonResident( piTexture vme ) = 0;
    virtual uint64_t  GetTextureHandle( piTexture vme ) = 0;

    //--- samplers ---
    virtual piSampler CreateSampler(TextureFilter filter, TextureWrap wrap, float anisotropy) = 0;
    virtual void      DestroySampler( piSampler obj ) = 0;
    virtual void      AttachSamplers( int num, piSampler vt0, piSampler vt1=0, piSampler vt2=0, piSampler vt3=0, piSampler vt4=0, piSampler vt5=0, piSampler vt6=0, piSampler vt7=0 ) = 0;
    virtual void      DettachSamplers( void ) = 0;

    // --- images ---
    virtual void      AttachImage(int unit, piTexture texture, int level, bool layered, int layer, piTextureFormat format ) = 0;

    //--- shaders ---
    virtual piShader  CreateShader( const piShaderOptions *options, const char *vs, const char *cs, const char *es, const char *gs, const char *fs, char *error) = 0;
	virtual piShader  CreateShaderBinary(const piShaderOptions *options, const uint8_t *vs, const int vs_len, const uint8_t *cs, const int cs_len, const uint8_t *es, const int es_len, const uint8_t *gs, const int gs_len, const uint8_t *fs, const int fs_len, char *error) = 0;
	virtual void      DestroyShader( piShader obj ) = 0;
    virtual void      AttachShader( piShader obj ) = 0;
    virtual void      DettachShader( void ) = 0;

    virtual piShader  CreateCompute(const piShaderOptions *options, const char *cs, char *error) = 0;

	virtual void      SetShaderConstant4F(const unsigned int pos, const float *value, int num) = 0;
	virtual void      SetShaderConstant3F(const unsigned int pos, const float *value, int num) = 0;
	virtual void      SetShaderConstant2F(const unsigned int pos, const float *value, int num) = 0;
	virtual void      SetShaderConstant1F(const unsigned int pos, const float *value, int num) = 0;
	virtual void      SetShaderConstant1I(const unsigned int pos, const int *value, int num) = 0;
	virtual void      SetShaderConstant1UI(const unsigned int pos, const unsigned int *value, int num) = 0;
	virtual void      SetShaderConstant2UI(const unsigned int pos, const unsigned int *value, int num) = 0;
	virtual void      SetShaderConstant3UI(const unsigned int pos, const unsigned int *value, int num) = 0;
	virtual void      SetShaderConstant4UI(const unsigned int pos, const unsigned int *value, int num) = 0;
	virtual void      SetShaderConstantMat4F(const unsigned int pos, const float *value, int num, bool transpose) = 0;
	virtual void      SetShaderConstantSampler(const unsigned int pos, int unit) = 0;
    virtual void      AttachShaderConstants(piBuffer obj, int unit) = 0;
    virtual void      AttachShaderBuffer(piBuffer obj, int unit) = 0;
    virtual void      DettachShaderBuffer(int unit) = 0;
    virtual void      AttachAtomicsBuffer(piBuffer obj, int unit) = 0;
    virtual void      DettachAtomicsBuffer(int unit) = 0;

    //--- buffers (vertex, index, shader constants, shader buffer, atomics, etc) ---
    virtual piBuffer  CreateBuffer(const void *data, unsigned int amount, BufferType mode, BufferUse use) = 0;
	virtual piBuffer  CreateStructuredBuffer(const void *data, unsigned int numElements, unsigned int elementSize, BufferType mode, BufferUse use) = 0;
    virtual void      DestroyBuffer(piBuffer) = 0;
    virtual void      UpdateBuffer(piBuffer obj, const void *data, int offset, int len) = 0;

    //--- arrays ---
    virtual piVertexArray CreateVertexArray(int numStreams, piBuffer vb0, const piRArrayLayout *streamLayout0, piBuffer vb1, const piRArrayLayout *streamLayout1, piBuffer eb, const IndexArrayFormat ebFormat) = 0;
    virtual void      DestroyVertexArray(piVertexArray obj) = 0;
    virtual void      AttachVertexArray(piVertexArray obj) = 0;
    virtual void      DettachVertexArray(void) = 0;

	virtual piVertexArray CreateVertexArray2(int numStreams, piBuffer vb0, const ArrayLayout2 *streamLayout0, piBuffer vb1, const ArrayLayout2 *streamLayout1, const void *shaderBinary, size_t shaderBinarySize, piBuffer ib, const IndexArrayFormat ebFormat) = 0;
	virtual void AttachVertexArray2(piVertexArray vme) = 0;
	virtual void DestroyVertexArray2(piVertexArray vme) = 0;
	
	//--- misc ---

    virtual void      DrawPrimitiveIndexed(PrimitiveType pt, int num, int numInstances, int baseVertex, int baseInstance) = 0;
    virtual void      DrawPrimitiveIndirect(PrimitiveType pt, piBuffer cmds, uint32_t offset, uint32_t num) = 0;
    virtual void      DrawPrimitiveNotIndexed(PrimitiveType pt, int first, int num, int numInstances) = 0;
    virtual void      DrawPrimitiveNotIndexedMultiple(PrimitiveType pt, const int *firsts, const int *counts, int num) = 0;
    virtual void      DrawPrimitiveNotIndexedIndirect(PrimitiveType pt, piBuffer cmds, int num) = 0;
	virtual void      DettachIndirectBuffer(void) = 0;

    virtual void      DrawUnitCube_XYZ_NOR(int numInstanced) = 0;
    virtual void      DrawUnitCube_XYZ(int numInstanced) = 0;
    virtual void      DrawUnitQuad_XY(int numInstanced) = 0;

    virtual void      ExecuteCompute( int ngx, int ngy, int ngz, int gsx, int gsy, int gsz ) = 0;


    //--- misc ---

    virtual void      SetPointSize( bool mode, float size ) = 0; // if false, the v/g shader decides
    virtual void      SetLineWidth( float size ) = 0;
    virtual void      PolygonOffset( bool mode, bool wireframe, float a, float b ) = 0;

    virtual void      RenderMemoryBarrier(BarrierType type ) = 0;

};

} // namespace piLibs
