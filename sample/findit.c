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
#include <math.h>

RGB_PACKED_IMAGE *rota( RGB_PACKED_IMAGE *template, int deg)
{
	int    i, j, m, n;
  float    x, y, p, q;
  double    r;
  float    c,s;
  int    xs = template->cols/2;
  int    ys = template->rows/2;
	int r_array[128][128];
	int g_array[128][128];
	int b_array[128][128];
  int    dr,dg,db;
	//int deg = 30;
	for (int a = 0; a < 128; a++){
		for (int b = 0; b < 128; b++){
			r_array[a][b]=template->p[a][b].r;
			g_array[a][b]=template->p[a][b].g;
			b_array[a][b]=template->p[a][b].b;
		}
	}

	RGB_PACKED_IMAGE *template3 = allocRGBPackedImage(template->cols , template->rows );
	/*
	for (int a = 0; a < 128; a++){
		for (int b = 0; b < 128; b++){
			template3->p[a][b].r = 255;
			template3->p[a][b].g = 255;
		}
	}
	*/
	//int *dr;
	//dr = (int*)malloc(sizeof(int)*20000);
	//RGB_PACKED_IMAGE *template3 = allocRGBPackedImage(300, 300);

  r = deg*3.141592/180.0;
  c = (float)cos(r);
  s = (float)sin(r);
  for (i = -ys; i < ys; i++) {
    for (j = -xs; j < xs; j++) {
      y = j*s + i*c;
      x = j*c - i*s;
      if (y > 0) m = (int)y;
      else m = (int)(y-1);
      if (x > 0) n = (int)x;
      else n = (int)(x-1);
      q = y - m;
      p = x - n;
			//printf("%d\n",m+1+ys);
			//printf("%d\n",n+1+xs);

      if ( (m >= -ys) && (m < ys-1) && (n >= -xs) && (n < xs-1) )
        dr = (int)(
						(1.0-q)*((1.0-p)*(template->p[m  +ys][n  +xs].r)
                               + p*(template->p[m  +ys][n+1+xs].r))
                       + q*((1.0-p)*(template->p[m+1+ys][n  +xs].r)
                                 + p*(template->p[m+1+ys][n+1+xs].r)));

				/*
        dr = (int)((1.0-q)*((1.0-p)*r_array[m  +ys][n  +xs]
                               + p*r_array[m  +ys][n+1+xs])
                       + q*((1.0-p)*r_array[m+1+ys][n  +xs]
                                 + p*r_array[m+1+ys][n+1+xs]))
																 */
				//dr = 3;
      else
        dr = 255;
      if (dr <   0) dr = 255;
      if (dr > 255) dr = 255;
			//丸め誤差で背景でなくなる画素への暫定対処
			if (dr == 254) dr = 255;
      template3->p[i+ys][j+xs].r = dr;
			//printf("%d\n",dr);
      if ( (m >= -ys) && (m < ys-1) && (n >= -xs) && (n < xs-1) )
        dg = (int)((1.0-q)*((1.0-p)*template->p[m  +ys][n  +xs].g
                               + p*template->p[m  +ys][n+1+xs].g)
                       + q*((1.0-p)*template->p[m+1+ys][n  +xs].g
                                 + p*template->p[m+1+ys][n+1+xs].g));
			/*
        dg = (int)((1.0-q)*((1.0-p)*g_array[m  +ys][n  +xs]
                               + p*g_array[m  +ys][n+1+xs])
                       + q*((1.0-p)*g_array[m+1+ys][n  +xs]
                                 + p*g_array[m+1+ys][n+1+xs]));
			*/
      else
        dg = 255;
      if (dg <   0) dg = 255;
      if (dg > 255) dg = 255;
			//丸め誤差で背景でなくなる画素への暫定対処
			if (dg == 254) dg = 255;
      template3->p[i+ys][j+xs].g = dg;
      if ( (m >= -ys) && (m < ys-1) && (n >= -xs) && (n < xs-1) )
        db = (int)((1.0-q)*((1.0-p)*template->p[m  +ys][n  +xs].b
                               + p*template->p[m  +ys][n+1+xs].b)
                       + q*((1.0-p)*template->p[m+1+ys][n  +xs].b
                                 + p*template->p[m+1+ys][n+1+xs].b));
			/*
        db = (int)((1.0-q)*((1.0-p)*b_array[m  +ys][n  +xs]
                               + p*b_array[m  +ys][n+1+xs])
                       + q*((1.0-p)*b_array[m+1+ys][n  +xs]
                                 + p*b_array[m+1+ys][n+1+xs]));
			*/

      else
        db = 255;
      if (db <   0) db = 255;
      if (db > 255) db = 255;
			//丸め誤差で背景でなくなる画素への暫定対処
			if (db == 254) db = 255;
      template3->p[i+ys][j+xs].b = db;
    }
  }
	//printf("%d\n",dr);
  // if(deg==1)
	// writeRGBPackedImage(template3, "tem3.ppm" );
  return template3;
	freeRGBPackedImage(template3);
}


RGB_PACKED_IMAGE *zoomreduction( RGB_PACKED_IMAGE *template, double scale)
{
	/* 縮小 */
	int xs = template->cols/2;
	int ys = template->rows/2;
	//double scale = 1.2;
	int xout = (template->cols * scale)/2;
	int yout = (template->rows * scale)/2;
	RGB_PACKED_IMAGE *template2 = allocRGBPackedImage(template->cols * scale,template->rows * scale);
  int i,j,m,n;

  for (i = -yout; i < yout; i++){
		for(j = -xout; j < xout; j++){
			if (i > 0) m = (int)(i/scale + 0.5);
				else m = (int)(i/scale - 0.5);
        if (j > 0) n = (int)(j/scale + 0.5);
        else n = (int)(j/scale - 0.5);
        if ( (m >= -ys) && (m < ys) && (n >= -xs) && (n < xs) ){
          template2->p[i+yout][j+xout].r = template->p[m+ys][n+xs].r;
          template2->p[i+yout][j+xout].g = template->p[m+ys][n+xs].g;
          template2->p[i+yout][j+xout].b = template->p[m+ys][n+xs].b;
				}else{
          template2->p[i+yout][j+xout].r = 255;
          template2->p[i+yout][j+xout].g = 255;
          template2->p[i+yout][j+xout].b = 255;
				}

		}
  }
	/* template2->data_p = template2->p; */
	/*
	template2->cols = (int)(scale * 2 * ys);
	template2->rows = (int)(scale * 2 * xs);
	*/
	template2->cols = (int)(template->cols * scale);
	template2->rows = (int)(template->rows * scale);
	/* writeRGBPackedImage( template2, "test.ppm"); */
	/* printf("%d",writeRGBPackedImage( template2, "test.ppm")); */
	//printf("%d",template2->cols);
	//printf("%d",template2->rows);

	/*writeRGBPackedImage( template, "sample.ppm"); */
	return template2;
	freeRGBPackedImage(template2);
}


// ガウシアンフィルタ
RGB_PACKED_IMAGE *g_filter( RGB_PACKED_IMAGE *image)
{

	//フィルター後画像の作成（とりあえず丸々コピー）
	RGB_PACKED_IMAGE *image_gfilt = allocRGBPackedImage(image->cols, image->rows);
	image_gfilt = image;

	int i, j, buffer_r, buffer_g, buffer_b, dr, dg, db;
	// printf("%d, %d", image->cols, image->rows); fflush(stdout);


	//端はそのままの値を使うことにする
	//5*5近傍ガウシアンフィルタを使用
	for(i=2; i < image->rows - 2; i++){
		for(j=2; j < image->cols - 2; j++){
			// printf(); fflush(stdout);
			// printf("%d,%d:",i, j); fflush(stdout);
			buffer_r = 	(int)image->p[i-2][j-2].r	*	1
								+	(int)image->p[i-2][j-1].r	*	4
								+	(int)image->p[i-2][j].r	*	6
								+	(int)image->p[i-2][j+1].r	*	4
								+	(int)image->p[i-2][j+2].r	*	1
								+	(int)image->p[i-1][j-2].r	*	4
								+	(int)image->p[i-1][j-1].r	*	16
								+	(int)image->p[i-1][j].r	*	24
								+	(int)image->p[i-1][j+1].r	*	16
								+	(int)image->p[i-1][j+2].r	*	4
								+	(int)image->p[i][j-2].r	*	6
								+	(int)image->p[i][j-1].r	*	24
								+	(int)image->p[i][j].r	*	36
								+	(int)image->p[i][j+1].r	*	24
								+	(int)image->p[i][j+2].r	*	6
								+	(int)image->p[i+1][j-2].r	*	4
								+	(int)image->p[i+1][j-1].r	*	16
								+	(int)image->p[i+1][j].r	*	24
								+	(int)image->p[i+1][j+1].r	*	16
								+	(int)image->p[i+1][j+2].r	*	4
								+	(int)image->p[i+2][j-2].r	*	1
								+	(int)image->p[i+2][j-1].r	*	4
								+	(int)image->p[i+2][j].r	*	6
								+	(int)image->p[i+2][j+1].r	*	4
								+	(int)image->p[i+2][j+2].r	*	1;

			buffer_g = 	(int)image->p[i-2][j-2].g	*	1
								+	(int)image->p[i-2][j-1].g	*	4
								+	(int)image->p[i-2][j].g	*	6
								+	(int)image->p[i-2][j+1].g	*	4
								+	(int)image->p[i-2][j+2].g	*	1
								+	(int)image->p[i-1][j-2].g	*	4
								+	(int)image->p[i-1][j-1].g	*	16
								+	(int)image->p[i-1][j].g	*	24
								+	(int)image->p[i-1][j+1].g	*	16
								+	(int)image->p[i-1][j+2].g	*	4
								+	(int)image->p[i][j-2].g	*	6
								+	(int)image->p[i][j-1].g	*	24
								+	(int)image->p[i][j].g	*	36
								+	(int)image->p[i][j+1].g	*	24
								+	(int)image->p[i][j+2].g	*	6
								+	(int)image->p[i+1][j-2].g	*	4
								+	(int)image->p[i+1][j-1].g	*	16
								+	(int)image->p[i+1][j].g	*	24
								+	(int)image->p[i+1][j+1].g	*	16
								+	(int)image->p[i+1][j+2].g	*	4
								+	(int)image->p[i+2][j-2].g	*	1
								+	(int)image->p[i+2][j-1].g	*	4
								+	(int)image->p[i+2][j].g	*	6
								+	(int)image->p[i+2][j+1].g	*	4
								+	(int)image->p[i+2][j+2].g	*	1;

			buffer_b = 	(int)image->p[i-2][j-2].b	*	1
								+	(int)image->p[i-2][j-1].b	*	4
								+	(int)image->p[i-2][j].b	*	6
								+	(int)image->p[i-2][j+1].b	*	4
								+	(int)image->p[i-2][j+2].b	*	1
								+	(int)image->p[i-1][j-2].b	*	4
								+	(int)image->p[i-1][j-1].b	*	16
								+	(int)image->p[i-1][j].b	*	24
								+	(int)image->p[i-1][j+1].b	*	16
								+	(int)image->p[i-1][j+2].b	*	4
								+	(int)image->p[i][j-2].b	*	6
								+	(int)image->p[i][j-1].b	*	24
								+	(int)image->p[i][j].b	*	36
								+	(int)image->p[i][j+1].b	*	24
								+	(int)image->p[i][j+2].b	*	6
								+	(int)image->p[i+1][j-2].b	*	4
								+	(int)image->p[i+1][j-1].b	*	16
								+	(int)image->p[i+1][j].b	*	24
								+	(int)image->p[i+1][j+1].b	*	16
								+	(int)image->p[i+1][j+2].b	*	4
								+	(int)image->p[i+2][j-2].b	*	1
								+	(int)image->p[i+2][j-1].b	*	4
								+	(int)image->p[i+2][j].b	*	6
								+	(int)image->p[i+2][j+1].b	*	4
								+	(int)image->p[i+2][j+2].b	*	1;

			dr = buffer_r / 256;
			dg = buffer_g / 256;
			db = buffer_b / 256;

			// printf ("(%d,%d,%d) ", dr, dg, db);

			if(dr > 254){
				image_gfilt->p[i][j].r = 254;
			} else if(dr < 0){
				image_gfilt->p[i][j].r = 0;
			} else {
				image_gfilt->p[i][j].r = dr;
				// printf("%d,", dr);
			}

			if(dg > 254){
				image_gfilt->p[i][j].g = 254;
			} else if(dg < 0){
				image_gfilt->p[i][j].g = 0;
			} else {
				image_gfilt->p[i][j].g = dg;
			}

			if(db > 254){
				image_gfilt->p[i][j].b = 254;
			} else if(db < 0){
				image_gfilt->p[i][j].b = 0;
			} else {
				image_gfilt->p[i][j].b = db;
			}

		}
	}
  // printf("test "); fflush(stdout);
	// writeRGBPackedImage( image_gfilt, "filtest.ppm" );

	return image_gfilt;

}

// ----------------- //
// ----------------- //
// ここまで前処理関数群 //
// ----------------- //
// ----------------- //


#ifdef __STDC__
int
findPattern( RGB_PACKED_IMAGE *template, RGB_PACKED_IMAGE *image,
		     double *cx, double *cy,
		     double *rotation, double *scaling )
#else
int
findPattern( template, image, cx, cy, rotation, scaling )
     //template = zoomreduction(template, 1.5);
     RGB_PACKED_IMAGE *template ;
     RGB_PACKED_IMAGE *image ;
     double *cx, *cy ;
     double *rotation ;
     double *scaling ;
#endif
{

  //変更箇所
	//-------------探索パラメータ--------------
  double scal_min = 0.6;//縮小下限
  double scal_max = 1.7;//拡大上限
  double scal_increment = 0.1;//刻み幅

  int rot_min = -10;//回転範囲
  int rot_max = 10;//回転範囲
  int rot_increment = 1;//刻み幅
	//---------------------------------------



  //--------------変数宣言など----------------
  RGB_PACKED_IMAGE *template_raw;
  template_raw = template; //テンプレート画像のオリジナル

	// 元画像のフィルタリング
	image = g_filter(image);

  int mindiff_all = 0x7fffffff ; //return用の変数
  double posx_all, posy_all, rotate_all, scale_all; //return用の変数

  int mindiff ;
  int posx, posy ;
  double rot, scal ;
  int xx, yy, dx, dy ;
  int x0, y0, x1, y1 ;
  int diff, pels, dr, dg, db ;
  RGB_PACKED_PIXEL *pixel ;
  //--------------------------------------






  ////////////////大体の場所を確保/////////////

  scal = 1.0;
  rot = 0;
  template = zoomreduction(template_raw, scal);//拡大縮小
  template = rota(template, rot);//回転


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
  //for ( yy = -y1 ; yy < image->rows - y0 ; yy++ ) {
    //for ( xx = -x1 ; xx < image->cols - x0 ; xx++ ) { // 変更前（templateの中心をimage内で全探索）
  for ( yy = -y0 ; yy < image->rows - y1 ; yy++ ) {
    for ( xx = -x0 ; xx < image->cols - x1 ; xx++ ) { // 変更後（templateがimage内に完全に含まれる前提で全探索）
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
  //暫定値の更新
  if ( mindiff < mindiff_all ){
		// 悪影響の可能性？（後の同条件での計算値と異なっている？）
		// midiff_all = mindiff ;
    posx_all = posx ;
    posy_all = posy ;
    rotate_all = rot ;
    scale_all = scal ;
  }
  printf("pels:%d ", pels);
  printf("mindiff:%d posx:%d posy:%d(scal:1.0, rot:0)\n", mindiff, posx, posy);

  //大体の場所を確保
  int around_posx = posx;
  int around_posy = posy;
  int error = 20;

  /////////////////大体の場所を確保/////////////


  // ----------------- //
	//   本格的な探索開始   //
	// ----------------- //

  //loop始まり
  for (scal = scal_min; scal <= scal_max ; scal += scal_increment){
    for (rot = rot_min; rot <= rot_max ; rot += rot_increment){

      template = zoomreduction(template_raw, scal);//拡大縮小
      template = rota(template, rot);//回転

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
      //for ( yy = -y1 ; yy < image->rows - y0 ; yy++ ) {
        //for ( xx = -x1 ; xx < image->cols - x0 ; xx++ ) { // 変更前（templateの中心をimage内で全探索）
      //for ( yy = -y0 ; yy < image->rows - y1 ; yy++ ) {
        //for ( xx = -x0 ; xx < image->cols - x1 ; xx++ ) { // 変更後（templateがimage内に完全に含まれる前提で全探索）
      for ( yy = around_posy - error ; yy < around_posy + error ; yy++ ) {
        for ( xx = around_posx - error ; xx < around_posx + error ; xx++ ) { // 変更後（大体検討をつけて探索）
          /*
           *  ある位置 (xx, yy) におけるふたつの画像間の差を求める.
           *  画像の差とは, ここでは R,G,B 値の差の絶対値の累積を用いた.
           */
          diff = 0 ; /* R,G,B それぞれの画素の差の累計 */
          pels = 0 ; /* 有効な比較を行った画素数 */
					// pels2 = 0;

          pixel = template->data_p ; /* テンプレートデータの先頭画素 */
          for ( dy = yy + y0 ; dy <= yy + y1 ; dy++ ) {
    	if ( dy >= 0 && dy < image->rows ) {
    	  /* 探索画像の外(上/下)に出てないことを確認し... */
    	  for ( dx = xx + x0 ; dx <= xx + x1 ; dx++, pixel++ ) {
    	    if ( dx >= 0 && dx < image->cols ) {
    	      /* 探索画像の外(左/右)に出てないことを確認し... */
						// pels2++;
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

      //暫定値の更新
      if ( mindiff < mindiff_all ){
        mindiff_all = mindiff ;
        posx_all = posx ;
        posy_all = posy ;
        rotate_all = rot ;
        scale_all = scal ;
      }
			printf("pels:%d ", pels);
      printf("mindiff:%d scale:%3.1f rotate:%4.1f posx:%d posy:%d\n", mindiff, scal, rot, posx, posy);
    }
    printf("scale:%3.1f loop finished.\n", scal);
  } //loop終わり


// ----------------- //
// mainに探索結果を返す //
// ----------------- //

  //returnの準備
  if ( mindiff_all == 0x7fffffff )
    return( HAS_ERROR ) ; /* 画像間の差が更新されていないので失敗と判断 */
    /*
     *  探索の結果を戻り値の引数に格納
     */
  *cx = (double)posx_all ;
  *cy = (double)posy_all ;
  *rotation = rotate_all ;
  *scaling = scale_all ;
  return( NO_ERROR ) ;

}
