public class genSpeedLut{
   private static final int LOW_FREQ = 490;
   private static final int HIGH_FREQ = 4000;
   private static final int LUT_SIZE = 256;
   private static final double BASE_FREQ = 15810000;

   public static void main(String[] args){
      float div_factor = (HIGH_FREQ - LOW_FREQ)/LUT_SIZE;
      System.out.println(div_factor);

      for(int i = 0; i < LUT_SIZE; i++){
         if(i== 0){
            System.out.print(0);
         }else{
            System.out.print((int)(BASE_FREQ /(i*div_factor + LOW_FREQ)));
         }

         if(i % 8 != 7){
            System.out.print(",");
         }else{
            System.out.println(",");
         }
      }
   }
}
