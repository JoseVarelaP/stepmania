#include "global.h"	// testing updates

/*
-----------------------------------------------------------------------------
 Class: ActorUtil

 Desc: See header.

 Copyright (c) 2001-2002 by the person(s) listed below.  All rights reserved.
	Chris Danford
-----------------------------------------------------------------------------
*/

#include "ActorUtil.h"
#include "Sprite.h"
#include "BitmapText.h"
#include "Model.h"
#include "IniFile.h"


Actor* MakeActor( CString sPath )
{
	CString sExt = GetExtension(sPath);
	sExt.MakeLower();
	

	if( sExt=="actor" )
	{
		// TODO: Check for recursive loading
		IniFile ini;
		ini.SetPath( sPath );
		ini.ReadFile();
		
		if( !ini.GetKey("Actor") )
			RageException::Throw( "The actor file '%s' is invalid.", sPath.c_str() );

		CString sFileName;
		ini.GetValue( "Actor", "File", sFileName );
		
		CString sNewPath = Dirname( sPath ) + sFileName;

		if( !DoesFileExist(sNewPath) )
			RageException::Throw( "The actor file '%s' references a file '%s' which doesn't exist.", sPath.c_str(), sNewPath.c_str() );

		sNewPath = DerefRedir( sNewPath );

		Actor* pActor = MakeActor( sNewPath );

		float f;
		if( ini.GetValue ( "Actor", "BaseRotationXDegrees", f ) )	pActor->SetBaseRotationX( f );
		if( ini.GetValue ( "Actor", "BaseRotationYDegrees", f ) )	pActor->SetBaseRotationY( f );
		if( ini.GetValue ( "Actor", "BaseRotationZDegrees", f ) )	pActor->SetBaseRotationZ( f );
		if( ini.GetValue ( "Actor", "BaseZoomX", f ) )				pActor->SetBaseZoomX( f );
		if( ini.GetValue ( "Actor", "BaseZoomY", f ) )				pActor->SetBaseZoomY( f );
		if( ini.GetValue ( "Actor", "BaseZoomZ", f ) )				pActor->SetBaseZoomZ( f );
			
		return pActor;
	}
	else if( sExt=="png" ||
		sExt=="jpg" || 
		sExt=="gif" || 
		sExt=="bmp" || 
		sExt=="avi" || 
		sExt=="mpeg" || 
		sExt=="mpg" ||
		sExt=="sprite" )
	{
		Sprite* pSprite = new Sprite;
		pSprite->Load( sPath );
		return pSprite;
	}
	else if( sExt=="ini" )
	{
		BitmapText* pBitmapText = new BitmapText;
		pBitmapText->LoadFromFont( sPath );
		return pBitmapText;
	}
	else if( sExt=="txt" )
	{
		Model* pModel = new Model;
		pModel->LoadMilkshapeAscii( sPath );
		return pModel;
	}

	RageException::Throw("File \"%s\" has unknown type, \"%s\"",
		sPath.c_str(), sExt.c_str() );
}

