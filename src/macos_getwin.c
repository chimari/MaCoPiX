// MaCoPiX : Mascot Constructive Pilot for X
//    macos_getwin.c
//       Get Window Info in macOS
//                                                     2019.6 K.Chimari

// GetWindowList
// A utility for printing out the name and windowId for every current window.
// Use in conjunction with 'screencapture -l' to automate screenshots.

// compile with gcc thusly:
// gcc -framework carbon -framework foundation -framework cocoa -x objective-c -std=c99 GetWindowList.c -o GetWindowList
#include "main.h"

#ifdef USE_OSX

#include <Carbon/Carbon.h>
#import <objc/Object.h>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>
#import <ApplicationServices/ApplicationServices.h>


void MacGetFocusWin(int *x, int *y, int *w, int *h){
  *x=-1;
  *y=-1;
  *w=-1;
  *h=-1;
  
  CFArrayRef windowArray = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
  NSMutableArray *windowsInMap = [NSMutableArray arrayWithCapacity:64];
  NSArray*  windows = (NSArray*)CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
  NSUInteger count = [windows count];
  
  CFArrayRef cwindowArray = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly | kCGWindowListExcludeDesktopElements, kCGNullWindowID);
  CFIndex windowCount = 0;
    
  for (NSUInteger i = 0; i < count; i++){
    NSDictionary*   nswindowsdescription = [windows objectAtIndex:i];
    NSNumber* windowid = (NSNumber*)[nswindowsdescription objectForKey:@"kCGWindowNumber"];
    NSString* windowName = (NSString*)[nswindowsdescription objectForKey:@"kCGWindowOwnerName"];
    CGRect rect;
    CFDictionaryRef windowdescription = (CFDictionaryRef)CFArrayGetValueAtIndex ((CFArrayRef)windowArray, i);
    
    
    if((windowid)&&(strcmp([windowName UTF8String], "MaCoPiX")!=0)){
      int layer;
      CFNumberGetValue(CFDictionaryGetValue(windowdescription, kCGWindowLayer), kCFNumberIntType, &layer);
      if(layer==0){
	CFDictionaryRef bounds = (CFDictionaryRef)CFDictionaryGetValue (windowdescription, kCGWindowBounds);
	if(bounds){
	  CGRectMakeWithDictionaryRepresentation(bounds, &rect);
	  *x=(int)rect.origin.x;
	  *y=(int)rect.origin.y;
	  *w=(int)rect.size.width;
	  *h=(int)rect.size.height;
	  if((x!=0)||(y!=0)){
	    //printf("%s:%s   ", [windowName UTF8String], [[windowid stringValue] UTF8String]);
	    break;
	  }
	}
      }
    }
    
  }
  CFRelease(windowArray);
}

void MacGetRootWin(int *w, int *h){
  *w=-1;
  *h=-1;
  
  // convert screen coordinates to global display coordinates
  NSRect r = [[[NSScreen screens] objectAtIndex: 0] frame];

  *w=(int)r.size.width;
  *h=(int)r.size.height;
}

/*
int main(int argc, char **argv) {
  int root_w, root_h;
  int x, y, w, h;
  sleep(5);

  MacGetRootWin(&root_w, &root_h);
  printf(" Root =   %dx%d\n",
	 root_w, root_h);
  
  MacGetFocusWin(&x, &y, &w, &h);
  printf("  (%d, %d) %dx%d\n",
	 x, y, w, h);
}
*/

#endif
