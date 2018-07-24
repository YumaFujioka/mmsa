/*
 *
 *	�����ŏЉ���T���v���v���O������, �e���v���[�g���T���摜��
 *	���ŉ��]���Ă�������, �g���k�����Ă����ꍇ�ɂ͑Ή����Ă��܂����D
 *
 *	������:
 *	�e���v���[�g�ɂ͔w�i����������, �T���摜���ł͔w�i�����͒T��
 *	�摜�̔w�i�ɒu���������Ă��܂�. ����������, �e���v���[�g�̑S�Ă�
 *	���f���T���摜���Ŕ��r�����̂ł͂Ȃ�, �w�i�łȂ������݂̂�
 *	���r�̑ΏۂƂ��ׂ��ł�. ���̃R���e�X�g�ł�, �w�i�F�����S�Ȕ�
 *	(R,G,B)=(255,255,255) �Œ��`��, �e���v���[�g�̓����ɂ͂��̐F��
 *	�����Ȃ��l�ɂ��Ă����܂��̂�, ���̒m�����O���ɂ����A���S���Y����
 *	�쐬���Ă�������. �ȉ���, �T���v���v���O�����ł͒l(255,255,255)��
 *	�����f�� don't care �ɂ����悤�ȍH�v���Ȃ����Ă��܂�.
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
			//�ۂߌ덷�Ŕw�i�łȂ��Ȃ����f�ւ̎b���Ώ�
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
			//�ۂߌ덷�Ŕw�i�łȂ��Ȃ����f�ւ̎b���Ώ�
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
			//�ۂߌ덷�Ŕw�i�łȂ��Ȃ����f�ւ̎b���Ώ�
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
	/* �k�� */
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


// �K�E�V�A���t�B���^
RGB_PACKED_IMAGE *g_filter( RGB_PACKED_IMAGE *image)
{

	//�t�B���^�[���摜�̍쐬�i�Ƃ肠�����ہX�R�s�[�j
	RGB_PACKED_IMAGE *image_gfilt = allocRGBPackedImage(image->cols, image->rows);
	image_gfilt = image;

	int i, j, buffer_r, buffer_g, buffer_b, dr, dg, db;
	// printf("%d, %d", image->cols, image->rows); fflush(stdout);


	//�[�͂��̂܂܂̒l���g�����Ƃɂ���
	//5*5�ߖT�K�E�V�A���t�B���^���g�p
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
// �����܂őO�����֐��Q //
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

  //�ύX�ӏ�
	//-------------�T���p�����[�^--------------
  double scal_min = 0.4;//�k������
  double scal_max = 1.7;//�g������
  double scal_increment = 0.1;//���ݕ�

  int rot_min = -25;//���]�͈�
  int rot_max = 25;//���]�͈�
  int rot_increment = 1;//���ݕ�
	//---------------------------------------



  //--------------�ϐ��錾�Ȃ�----------------
  RGB_PACKED_IMAGE *template_raw;
  template_raw = template; //�e���v���[�g�摜�̃I���W�i��

	// ���摜�̃t�B���^�����O
	image = g_filter(image);

  int mindiff_all = 0x7fffffff ; //return�p�̕ϐ�
  double posx_all, posy_all, rotate_all, scale_all; //return�p�̕ϐ�

  int mindiff ;
  int posx, posy ;
  double rot, scal ;
  int xx, yy, dx, dy ;
  int x0, y0, x1, y1 ;
  int diff, pels, dr, dg, db ;
  RGB_PACKED_PIXEL *pixel ;
  //--------------------------------------






  ////////////////���̂̏ꏊ���m��/////////////

  scal = 1.0;
  rot = 0;
  template = zoomreduction(template_raw, scal);//�g���k��
  template = rota(template, rot);//���]


  /*
   *  �e���v���[�g�̒��S���猩��, �e���v���[�g�̍����ƉE���̍��W
   *  (x0,y0) �� (x1, y1) �����炩���ߋ��߂Ă���.
   */
  x0 = -( template->cols / 2 ) ;
  y0 = -( template->rows / 2 ) ;
  x1 = ( template->cols - 1 )/ 2 ;
  y1 = ( template->rows - 1 )/ 2 ;
  /*
   *  �e���v���[�g�𓖂Ă͂߂��ʒu���T���摜�̑S�͈͂Ɉړ������Ȃ���,
   *  �e���v���[�g�ƒT���摜�̍��������Ƃ��������ʒu (posx, posy) ��
   *  ���t����.
   */
  mindiff = 0x7fffffff ;
  posx=0; posy=0;
  //for ( yy = -y1 ; yy < image->rows - y0 ; yy++ ) {
    //for ( xx = -x1 ; xx < image->cols - x0 ; xx++ ) { // �ύX�O�itemplate�̒��S��image���őS�T���j
  for ( yy = -y0 ; yy < image->rows - y1 ; yy++ ) {
    for ( xx = -x0 ; xx < image->cols - x1 ; xx++ ) { // �ύX���itemplate��image���Ɋ��S�Ɋ܂܂����O���őS�T���j
      /*
       *  �����ʒu (xx, yy) �ɂ������ӂ��̉摜�Ԃ̍������߂�.
       *  �摜�̍��Ƃ�, �����ł� R,G,B �l�̍��̐��Βl�̗ݐς��p����.
       */
      diff = 0 ; /* R,G,B ���ꂼ���̉��f�̍��̗݌v */
      pels = 0 ; /* �L���Ȕ��r���s�������f�� */
      pixel = template->data_p ; /* �e���v���[�g�f�[�^�̐擪���f */
      for ( dy = yy + y0 ; dy <= yy + y1 ; dy++ ) {
  if ( dy >= 0 && dy < image->rows ) {
    /* �T���摜�̊O(��/��)�ɏo�ĂȂ����Ƃ��m�F��... */
    for ( dx = xx + x0 ; dx <= xx + x1 ; dx++, pixel++ ) {
      if ( dx >= 0 && dx < image->cols ) {
        /* �T���摜�̊O(��/�E)�ɏo�ĂȂ����Ƃ��m�F��... */
        if ( pixel->r != 255 || pixel->g != 255 || pixel->b != 255 ) {
    /* �e���v���[�g���f���w�i�łȂ����Ƃ��m�F��... */
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
    /* �e���v���[�g�����s�ǂݔ��΂����̂�, �|�C���^�𒲐����� */
    pixel += template->cols ;
  }
      }
      if ( pels ) { /* �L���ɍ����ݐς����Ă����ꍇ�ɂ�... */
  diff /= pels ; /* ���f�̍��̗݌v���L�����f���Ŋ����Đ��K������ */
  /*
   *  �����܂ł̌��ʂƔ��r��, �������������΂��̈ʒu���̗p����.
   */
  if ( diff < mindiff ) {
    mindiff = diff ;
    posx = xx ;
    posy = yy ;
  }
      }
    }
  }
  //�b���l�̍X�V
  if ( mindiff < mindiff_all ){
		// ���e���̉\���H�i���̓������ł̌v�Z�l�ƈقȂ��Ă����H�j
		// midiff_all = mindiff ;
    posx_all = posx ;
    posy_all = posy ;
    rotate_all = rot ;
    scale_all = scal ;
  }
  // printf("pels:%d ", pels);

	// �ڍ׌��ʕ\��
  // printf("mindiff:%d posx:%d posy:%d(scal:1.0, rot:0)\n", mindiff, posx, posy);

  //���̂̏ꏊ���m��
  int around_posx = posx;
  int around_posy = posy;
	// �����܂��T���͈�
  int error = 20;

  /////////////////���̂̏ꏊ���m��/////////////


  // ----------------- //
	//   �{�i�I�ȒT���J�n   //
	// ----------------- //

  //loop�n�܂�
  for (scal = scal_min; scal <= scal_max ; scal += scal_increment){
    for (rot = rot_min; rot <= rot_max ; rot += rot_increment){

      template = zoomreduction(template_raw, scal);//�g���k��
      template = rota(template, rot);//���]

      /*
       *  �e���v���[�g�̒��S���猩��, �e���v���[�g�̍����ƉE���̍��W
       *  (x0,y0) �� (x1, y1) �����炩���ߋ��߂Ă���.
       */
      x0 = -( template->cols / 2 ) ;
      y0 = -( template->rows / 2 ) ;
      x1 = ( template->cols - 1 )/ 2 ;
      y1 = ( template->rows - 1 )/ 2 ;

      /*
       *  �e���v���[�g�𓖂Ă͂߂��ʒu���T���摜�̑S�͈͂Ɉړ������Ȃ���,
       *  �e���v���[�g�ƒT���摜�̍��������Ƃ��������ʒu (posx, posy) ��
       *  ���t����.
       */
      mindiff = 0x7fffffff ;
      posx=0; posy=0;
      //for ( yy = -y1 ; yy < image->rows - y0 ; yy++ ) {
        //for ( xx = -x1 ; xx < image->cols - x0 ; xx++ ) { // �ύX�O�itemplate�̒��S��image���őS�T���j
      //for ( yy = -y0 ; yy < image->rows - y1 ; yy++ ) {
        //for ( xx = -x0 ; xx < image->cols - x1 ; xx++ ) { // �ύX���itemplate��image���Ɋ��S�Ɋ܂܂����O���őS�T���j
      for ( yy = around_posy - error ; yy < around_posy + error ; yy++ ) {
        for ( xx = around_posx - error ; xx < around_posx + error ; xx++ ) { // �ύX���i���̌��������ĒT���j
          /*
           *  �����ʒu (xx, yy) �ɂ������ӂ��̉摜�Ԃ̍������߂�.
           *  �摜�̍��Ƃ�, �����ł� R,G,B �l�̍��̐��Βl�̗ݐς��p����.
           */
          diff = 0 ; /* R,G,B ���ꂼ���̉��f�̍��̗݌v */
          pels = 0 ; /* �L���Ȕ��r���s�������f�� */
					// pels2 = 0;

          pixel = template->data_p ; /* �e���v���[�g�f�[�^�̐擪���f */
          for ( dy = yy + y0 ; dy <= yy + y1 ; dy++ ) {
    	if ( dy >= 0 && dy < image->rows ) {
    	  /* �T���摜�̊O(��/��)�ɏo�ĂȂ����Ƃ��m�F��... */
    	  for ( dx = xx + x0 ; dx <= xx + x1 ; dx++, pixel++ ) {
    	    if ( dx >= 0 && dx < image->cols ) {
    	      /* �T���摜�̊O(��/�E)�ɏo�ĂȂ����Ƃ��m�F��... */
						// pels2++;
    	      if ( pixel->r != 255 || pixel->g != 255 || pixel->b != 255 ) {
    		/* �e���v���[�g���f���w�i�łȂ����Ƃ��m�F��... */
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
    	  /* �e���v���[�g�����s�ǂݔ��΂����̂�, �|�C���^�𒲐����� */
    	  pixel += template->cols ;
    	}
          }
          if ( pels ) { /* �L���ɍ����ݐς����Ă����ꍇ�ɂ�... */
    	diff /= pels ; /* ���f�̍��̗݌v���L�����f���Ŋ����Đ��K������ */
    	/*
    	 *  �����܂ł̌��ʂƔ��r��, �������������΂��̈ʒu���̗p����.
    	 */
    	if ( diff < mindiff ) {
    	  mindiff = diff ;
    	  posx = xx ;
    	  posy = yy ;
    	}
          }
        }
      }

      //�b���l�̍X�V
      if ( mindiff < mindiff_all ){
        mindiff_all = mindiff ;
        posx_all = posx ;
        posy_all = posy ;
        rotate_all = rot ;
        scale_all = scal ;
      }
			// printf("pels:%d ", pels);

			// �ڍ׌��ʕ\��
      // printf("mindiff:%d scale:%3.1f rotate:%4.1f posx:%d posy:%d\n", mindiff, scal, rot, posx, posy);
    }
    // printf("scale:%3.1f loop finished.\n", scal);
  } //loop�I����


// ----------------- //
// main�ɒT�����ʂ��Ԃ� //
// ----------------- //

  //return�̏���
  if ( mindiff_all == 0x7fffffff )
    return( HAS_ERROR ) ; /* �摜�Ԃ̍����X�V�����Ă��Ȃ��̂Ŏ��s�Ɣ��f */
    /*
     *  �T���̌��ʂ��߂��l�̈����Ɋi�[
     */
  *cx = (double)posx_all ;
  *cy = (double)posy_all ;
  *rotation = rotate_all ;
  *scaling = scale_all ;
  return( NO_ERROR ) ;

}
