/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 3.0 for Linux OS                    *
 * File:    MessageBar.cpp                      *
 * Date:    July 6, 2016                        *
 * Author:  Chris Sterne                        *
 *                                              *
 * This class displays one-line messages.       *
 *----------------------------------------------*/

#include <glibmm/i18n.h>
#include "MessageBar.h"

//*--------------------------------------------------------------*
//* This method displays a string description of a map location. *
//*--------------------------------------------------------------*
//* aLocation: Map location to be displayed.                     *
//*--------------------------------------------------------------*

void CMessageBar::SetLocation( const CMapLocation& aLocation )
{
	Glib::ustring String =
		Glib::ustring::compose( _("East = %1   North = %2   Above = %3"),
														 aLocation.iEast,
    	                     	 aLocation.iNorth,
     	                       aLocation.iAbove );
	set_label( String );
	return;
}