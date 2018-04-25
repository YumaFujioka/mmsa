/* 
 *
 *	�����ŏЉ��T���v���v���O������, �e���v���[�g���T���摜��
 *	���ŉ�]���Ă�������, �g��k�����Ă���ꍇ�ɂ͑Ή����Ă��܂���D
 *
 *	������: 
 *	�e���v���[�g�ɂ͔w�i����������, �T���摜���ł͔w�i�����͒T��
 *	�摜�̔w�i�ɒu��������Ă��܂�. ����������, �e���v���[�g�̑S�Ă�
 *	��f��T���摜���Ŕ�r����̂ł͂Ȃ�, �w�i�łȂ������݂̂�
 *	��r�̑ΏۂƂ��ׂ��ł�. ���̃R���e�X�g�ł�, �w�i�F�����S�Ȕ�
 *	(R,G,B)=(255,255,255) �Œ�`��, �e���v���[�g�̓����ɂ͂��̐F��
 *	����Ȃ��l�ɂ��Ă���܂��̂�, ���̒m����O��ɂ����A���S���Y����
 *	�쐬���Ă�������. �ȉ���, �T���v���v���O�����ł͒l(255,255,255)��
 *	����f�� don't care �ɂ���悤�ȍH�v���Ȃ���Ă��܂�.
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
   *  �e���v���[�g�̒��S���猩��, �e���v���[�g�̍���ƉE���̍��W
   *  (x0,y0) �� (x1, y1) �����炩���ߋ��߂Ă���.
   */
  x0 = -( template->cols / 2 ) ;
  y0 = -( template->rows / 2 ) ;
  x1 = ( template->cols - 1 )/ 2 ;
  y1 = ( template->rows - 1 )/ 2 ;

  /*
   *  �e���v���[�g�𓖂Ă͂߂�ʒu��T���摜�̑S�͈͂Ɉړ������Ȃ���,
   *  �e���v���[�g�ƒT���摜�̍��������Ƃ��������ʒu (posx, posy) ��
   *  ���t����.
   */
  mindiff = 0x7fffffff ;
  posx=0; posy=0;
  for ( yy = -y1 ; yy < image->rows - y0 ; yy++ ) {
    for ( xx = -x1 ; xx < image->cols - x0 ; xx++ ) {

      /*
       *  ����ʒu (xx, yy) �ɂ�����ӂ��̉摜�Ԃ̍������߂�.
       *  �摜�̍��Ƃ�, �����ł� R,G,B �l�̍��̐�Βl�̗ݐς�p����.
       */
      diff = 0 ; /* R,G,B ���ꂼ��̉�f�̍��̗݌v */
      pels = 0 ; /* �L���Ȕ�r���s������f�� */
      pixel = template->data_p ; /* �e���v���[�g�f�[�^�̐擪��f */
      for ( dy = yy + y0 ; dy <= yy + y1 ; dy++ ) {
	if ( dy >= 0 && dy < image->rows ) {
	  /* �T���摜�̊O(��/��)�ɏo�ĂȂ����Ƃ��m�F��... */
	  for ( dx = xx + x0 ; dx <= xx + x1 ; dx++, pixel++ ) {
	    if ( dx >= 0 && dx < image->cols ) {
	      /* �T���摜�̊O(��/�E)�ɏo�ĂȂ����Ƃ��m�F��... */
	      if ( pixel->r != 255 || pixel->g != 255 || pixel->b != 255 ) {
		/* �e���v���[�g��f���w�i�łȂ����Ƃ��m�F��... */
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
	  /* �e���v���[�g����s�ǂݔ�΂����̂�, �|�C���^�𒲐����� */
	  pixel += template->cols ;
	}
      }
      if ( pels ) { /* �L���ɍ����ݐς���Ă����ꍇ�ɂ�... */
	diff /= pels ; /* ��f�̍��̗݌v��L����f���Ŋ����Đ��K������ */
	/*
	 *  ����܂ł̌��ʂƔ�r��, ������������΂��̈ʒu���̗p����.
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
    return( HAS_ERROR ) ; /* �摜�Ԃ̍����X�V����Ă��Ȃ��̂Ŏ��s�Ɣ��f */
  
  /*
   *  �T���̌��ʂ�߂�l�̈����Ɋi�[
   */
  *cx = (double)posx ;
  *cy = (double)posy ;
  *rotation = 0.0 ; 
  *scaling = 1.0 ;
  return( NO_ERROR ) ;
}
