/* 
 *	���̃v���O������, ���炩���ߗ^����ꂽ�摜�p�^�[��(�ȉ����̉摜��
 *	���Ƃ��e���v���[�g�摜�ƌĂт܂�)��, �ʂ̑傫�ȉ摜(�ȉ����̉摜�̂���
 *	��T���Ώۉ摜�ƌĂт܂�)����T���܂�. ���t���������ʂ�, �T���Ώۉ摜��
 *	��Ƀe���v���[�g�摜�̊O�g�ɑ������钷���`���d�ˏ������邱�Ƃ�, 
 *	������悤�ɂ��܂�. 
 *
 *	�摜��, netpbm (pbmplus) �ŗp������J���[�摜�t�H�[�}�b�g `ppm' ��
 *	�p���Ă��܂�. 
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include "pnmimg.h"

#ifdef __STDC__
int
main( int argc, char *argv[] )
#else
int
main( argc, argv )
     int argc ;
     char *argv[] ;
#endif
{
  /*
   *  ���̃v���O�����͂R�̃R�}���h���C���������w�肵�ē��삵�܂�.
   *  ��P�̈�����, �e���v���[�g�̉摜�t�@�C��(PPM �t�H�[�}�b�g)�ł���,
   *  ��Q�̈�����, �T���Ώۉ摜�̃t�@�C���ł�. �������ȗ����ꂽ�ꍇ�ɂ�,
   *  ��P�����̑���� "template.ppm" �Ƃ������O��, ��Q�����̑����
   *  "image.ppm" �Ƃ������O���p�����܂�.
   *  �X��, ��R�̈������w�肷���, �T�����ʂ��o�͂���摜�t�@�C����
   *  ���O���w�肷�邱�Ƃ��o���܂�. �w�肵�Ȃ����, "result.ppm" �Ƃ���
   *  ���O���p�����܂�.
   */
  char *name_tmp = "template.ppm" ;
  char *name_img = "image.ppm" ;
  char *name_out = "result.ppm" ;
  RGB_PACKED_IMAGE *template ;
  RGB_PACKED_IMAGE *image ;
  double cx, cy, rotation, scaling ;
  int icx, icy, irot, isx, isy ;
  int ret ;
  extern int findPattern __P(( RGB_PACKED_IMAGE*, RGB_PACKED_IMAGE*, double*, double*, double*, double* )) ;
  extern RGB_PACKED_IMAGE* zoomreduction(RGB_PACKED_IMAGE*,double);
  extern RGB_PACKED_IMAGE* rota(RGB_PACKED_IMAGE*,int);

  if ( argc > 1 ) name_tmp = argv[1] ;
  if ( argc > 2 ) name_img = argv[2] ;
  if ( argc > 3 ) name_out = argv[3] ;

  if (!( template = readRGBPackedImage( name_tmp ))) {
    printError( name_tmp ) ;
    return(1) ;
  }
  if (!( image = readRGBPackedImage( name_img ))) {
    printError( name_img ) ;
    return(1) ;
  }

  /*
   *  �Q���̉摜���ǂݍ��܂�܂���. ��������, �����̉摜��
   *  �p�^�[����T������֐� findPattern() �ɓn����,
   *  �T�����s���܂�. ���ʂ͌��t�������e���v���[�g�̉摜��
   *  ���S�̍��W ( cx, cy ) �ƃe���v���[�g�̉�]�p rotation,
   *  ����ъg��/�k���� scaling �Ƃɓ����, �Ԃ���܂�. ������
   *  �ϐ��� double �\���ł�. �܂�, scaling �͌��t�����p�^�[����
   *  �傫���ƃe���v���[�g�̑傫���Ƃ̔�ŕ\����, rotation ��
   *  ��ʂɑ΂��Ĕ����v����𐳂Ƃ��� degree (�x)�P�ʂŕ\�����܂�.
   *  ���̊֐���, ���܂����t���������� NO_ERROR (0) ��Ԃ�,
   *  �����łȂ���� HAS_ERROR (-1) ��Ԃ��悤�ɂȂ��Ă��܂�.
   */
	template = rota(template,20);
	//template = zoomreduction(template,1.5); 
	writeRGBPackedImage(template,"sample5.ppm");
  ret = findPattern( template, image, 
			      &cx, &cy, &rotation, &scaling ) ;
	/* scaling = 1.1; */
  if ( ret == HAS_ERROR ) {
    printError( "findPattern" ) ;
    printf( "%s is failed to be searched in %s\n", name_tmp, name_img ) ;
    return(1);
  }

  /*
   *  �T���������ʂ����̒T���摜�̏�ɒ����`�̘g�ŕ\�����܂�.
   */
  icx = (int)( cx + .5 ) ;
  icy = (int)( cy + .5 ) ;
  isx = (int)( scaling * template->cols + .5 ) ;
  isy = (int)( scaling * template->rows + .5 ) ;
	/*
  isx = (int)( template->cols + .5 ) ;
  isy = (int)( template->rows + .5 ) ;
	*/
  irot = (int)( rotation + .5 ) ;
  setRGBMarkColor( 255, 255, 0 ) ;
  drawRotatedRectangleRGBPackedImage( image, icx, icy, isx, isy, irot ) ;

  /*
   *  ���ʂ�[���ɏo�͂�, �摜���t�@�C���ɃZ�[�u���܂�.
   */
  printf( "%s is successfully searched in %s\n", name_tmp, name_img ) ;
  printf( "location (%5.1lf,%5.1lf), rotation %6.1lf, scaling %4.2lf\n",
	  cx, cy, rotation, scaling ) ;

  if ( writeRGBPackedImage( image, name_out ) == HAS_ERROR ) {
    printError( name_out ) ;
    return(1) ;
  }
  printf( "result image is saved in %s\n", name_out ) ;

  return(0) ;
}
