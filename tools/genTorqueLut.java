public class genTorqueLut{
   public static void main(String[] args){
      genLutLowToHigh(100, 200, 256, false);
   }

   /** zeroStart means the value LUT[0] is 0 no matter what **/
   public static void genLutLowToHigh(int low, int high, int size, boolean zeroStart){
      double div_factor = (high - low)/(double)size;

      for(int i = 0; i < size; i++){
         if(i== 0 && zeroStart){
            System.out.print(0);
         }else{
            System.out.print((int)(i*div_factor + low));
         }

         if(i % 8 != 7){
            System.out.print(",");
         }else{
            System.out.println(",");
         }
      }

   }

   public static void genLutHighToLow(int high, int low, int size, boolean zeroStart){
      double div_factor = (high - low)/(double)size;

      for(int i = 0; i < size; i++){
         if(i== 0 && zeroStart){
            System.out.print(0);
         }else{
            System.out.print((int)(high - i*div_factor));
         }

         if(i % 8 != 7){
            System.out.print(",");
         }else{
            System.out.println(",");
         }
      }

   }
}
