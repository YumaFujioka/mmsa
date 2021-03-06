/* 
 *
 *	ここで紹介するサンプルプログラムは, テンプレートが探索画像の
 *	中で回転してあったり, 拡大縮小している場合には対応していません．
 *
 *	ご注意: 
 *	テンプレートには背景部分があり, 探索画像中では背景部分は探索
 *	画像の背景に置き換わっています. したがって, テンプレートの全ての
 *	画素を探索画像中で比較するのではなく, 背景でない部分のみを
 *	比較の対象とすべきです. このコンテストでは, 背景色を完全な白
 *	(R,G,B)=(255,255,255) で定義し, テンプレートの内部にはこの色が
 *	現れない様にしてありますので, この知識を前提にしたアルゴリズムを
 *	作成してください. 以下の, サンプルプログラムでは値(255,255,255)を
 *	持つ画素を don't care にするような工夫がなされています.
 */
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include "pnmimg.h"

#ifdef __STDC__
int
findPattern( RGB_PACKED_IMAGE *template, RGB_PACKED_IMAGE *image,
		     double *cx, double *cy, 
		     double *rotation, double *scaling )
#else
int
findPattern( template, image, cx, cy, rotation, scaling )
     RGB_PACKED_IMAGE *template ;
     RGB_PACKED_IMAGE *image ;
     double *cx, *cy ;
     double *rotation ;
     double *scaling ;
#endif
{
  int mindiff ;
  int posx, posy, rot, scal ;
  int xx, yy, dx, dy ;
  int x0, y0, x1, y1 ;
  int diff, pels, dr, dg, db ;
  RGB_PACKED_PIXEL *pixel ;

  /*
   *  テンプレートの中心から見た, テンプレートの左上と右下の座標
   *  (x0,y0) と (x1, y1) をあらかじめ求めておく.
   */
  x0 = -( template->cols / 2 ) ;
  y0 = -( template->rows / 2 ) ;
  x1 = ( template->cols - 1 )/ 2 ;
  y1 = ( template->rows - 1 )/ 2 ;

  /*
   *  テンプレートを当てはめる位置を探索画像の全範囲に移動させながら,
   *  テンプレートと探索画像の差がもっとも小さい位置 (posx, posy) を
   *  見付ける.
   */
  mindiff = 0x7fffffff ;
  posx=0; posy=0;
  for ( yy = -y1 ; yy < image->rows - y0 ; yy++ ) {
    for ( xx = -x1 ; xx < image->cols - x0 ; xx++ ) {

      /*
       *  ある位置 (xx, yy) におけるふたつの画像間の差を求める.
       *  画像の差とは, ここでは R,G,B 値の差の絶対値の累積を用いた.
       */
      diff = 0 ; /* R,G,B それぞれの画素の差の累計 */
      pels = 0 ; /* 有効な比較を行った画素数 */
      pixel = template->data_p ; /* テンプレートデータの先頭画素 */
      for ( dy = yy + y0 ; dy <= yy + y1 ; dy++ ) {
	if ( dy >= 0 && dy < image->rows ) {
	  /* 探索画像の外(上/下)に出てないことを確認し... */
	  for ( dx = xx + x0 ; dx <= xx + x1 ; dx++, pixel++ ) {
	    if ( dx >= 0 && dx < image->cols ) {
	      /* 探索画像の外(左/右)に出てないことを確認し... */
	      if ( pixel->r != 255 || pixel->g != 255 || pixel->b != 255 ) {
		/* テンプレート画素が背景でないことを確認し... */
		pels ++ ;
		if (( dr = image->p[dy][dx].r - pixel->r ) < 0 ) dr = -dr ;
		if (( dg = image->p[dy][dx].g - pixel->g ) < 0 ) dg = -dg ;
		if (( db = image->p[dy][dx].b - pixel->b ) < 0 ) db = -db ;
		diff += ( dr + dg + db ) ;
	      }
	    }
	  }
	}
	else {
	  /* テンプレートを一行読み飛ばしたので, ポインタを調整する */
	  pixel += template->cols ;
	}
      }
      if ( pels ) { /* 有効に差が累積されていた場合には... */
	diff /= pels ; /* 画素の差の累計を有効画素数で割って正規化する */
	/*
	 *  これまでの結果と比較し, 差が小さければその位置を採用する.
	 */
	if ( diff < mindiff ) {
	  mindiff = diff ;
	  posx = xx ;
	  posy = yy ;
	}
      }
    }
  }
  if ( mindiff == 0x7fffffff )
    return( HAS_ERROR ) ; /* 画像間の差が更新されていないので失敗と判断 */
  
  /*
   *  探索の結果を戻り値の引数に格納
   */
  *cx = (double)posx ;
  *cy = (double)posy ;
  *rotation = 0.0 ; 
  *scaling = 1.0 ;
  return( NO_ERROR ) ;
}
