**************************************************************************
      subroutine upcase(w)
***************************************************************************
C     Converts string (up to first blank) to upper case.

      character*(*) w
      do 10 i=1,len(w)
         if(w(i:i).eq.' ') go to 20
         j=ichar(w(i:i))
         if(j.ge.97.and.j.le.122) w(i:i)=char(j-32)
 10   continue
 20   return
      end

C     and from citem.f

      subroutine citem(line,ll,j1,item,li)

c     Searches line(j1:ll) for next .not.(space.or.tab)
c     Returns item of non-blank length li, and j1 = index of following character.
c     RNM March, 1995

      implicit none
      integer ll,j,jn,j1,li
      character line*(*),item*(*)

      item=' '
      do j=j1,ll
         if(line(j:j).ne.' '.and.ichar(line(j:j)).ne.9)go to 10
      enddo
      li=0                      ! No non-space character found
      return

 10   jn=j

      do j=jn,ll
         if(line(j:j).eq.' '.or.ichar(line(j:j)).eq.9)go to 20
      enddo
      j1=ll
      go to 30

 20   j1=j-1

 30   li=j1-jn+1
      item(1:li)=line(jn:j1)
      j1=j1+1

      return
      end
