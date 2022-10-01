; ModuleID = 'main.c'
source_filename = "main.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: nofree norecurse nosync nounwind optsize readnone uwtable
define dso_local i32 @func1(i32 noundef %0, i32 noundef %1, i32 noundef %2) local_unnamed_addr #0 {
  %4 = icmp sgt i32 %0, %1
  br i1 %4, label %32, label %5

5:                                                ; preds = %3
  %6 = add i32 %1, 1
  %7 = sub i32 %6, %0
  %8 = add i32 %7, 3
  %9 = and i32 %8, -4
  %10 = add i32 %7, -1
  %11 = insertelement <4 x i32> poison, i32 %10, i64 0
  %12 = shufflevector <4 x i32> %11, <4 x i32> poison, <4 x i32> zeroinitializer
  %13 = insertelement <4 x i32> <i32 poison, i32 1, i32 1, i32 1>, i32 %2, i64 0
  %14 = insertelement <4 x i32> poison, i32 %0, i64 0
  %15 = shufflevector <4 x i32> %14, <4 x i32> poison, <4 x i32> zeroinitializer
  %16 = add <4 x i32> %15, <i32 0, i32 1, i32 2, i32 3>
  br label %17

17:                                               ; preds = %17, %5
  %18 = phi i32 [ 0, %5 ], [ %22, %17 ]
  %19 = phi <4 x i32> [ %13, %5 ], [ %21, %17 ]
  %20 = phi <4 x i32> [ %16, %5 ], [ %23, %17 ]
  %21 = mul <4 x i32> %19, %20
  %22 = add i32 %18, 4
  %23 = add <4 x i32> %20, <i32 4, i32 4, i32 4, i32 4>
  %24 = icmp eq i32 %22, %9
  br i1 %24, label %25, label %17, !llvm.loop !5

25:                                               ; preds = %17
  %26 = insertelement <4 x i32> poison, i32 %18, i64 0
  %27 = shufflevector <4 x i32> %26, <4 x i32> poison, <4 x i32> zeroinitializer
  %28 = or <4 x i32> %27, <i32 0, i32 1, i32 2, i32 3>
  %29 = icmp ugt <4 x i32> %28, %12
  %30 = select <4 x i1> %29, <4 x i32> %19, <4 x i32> %21
  %31 = call i32 @llvm.vector.reduce.mul.v4i32(<4 x i32> %30)
  br label %32

32:                                               ; preds = %25, %3
  %33 = phi i32 [ %2, %3 ], [ %31, %25 ]
  ret i32 %33
}

; Function Attrs: nofree nounwind optsize uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
  %1 = alloca i32, align 4
  %2 = bitcast i32* %1 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %2) #5
  %3 = call i32 (i8*, ...) @__isoc99_scanf(i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32* noundef nonnull %1) #6
  %4 = load i32, i32* %1, align 4, !tbaa !8
  %5 = icmp slt i32 %4, 2
  br i1 %5, label %27, label %6

6:                                                ; preds = %0
  %7 = add i32 %4, 2
  %8 = and i32 %7, -4
  %9 = add i32 %4, -2
  %10 = insertelement <4 x i32> poison, i32 %9, i64 0
  %11 = shufflevector <4 x i32> %10, <4 x i32> poison, <4 x i32> zeroinitializer
  br label %12

12:                                               ; preds = %12, %6
  %13 = phi i32 [ 0, %6 ], [ %17, %12 ]
  %14 = phi <4 x i32> [ <i32 1, i32 1, i32 1, i32 1>, %6 ], [ %16, %12 ]
  %15 = phi <4 x i32> [ <i32 2, i32 3, i32 4, i32 5>, %6 ], [ %18, %12 ]
  %16 = mul <4 x i32> %15, %14
  %17 = add i32 %13, 4
  %18 = add <4 x i32> %15, <i32 4, i32 4, i32 4, i32 4>
  %19 = icmp eq i32 %17, %8
  br i1 %19, label %20, label %12, !llvm.loop !12

20:                                               ; preds = %12
  %21 = insertelement <4 x i32> poison, i32 %13, i64 0
  %22 = shufflevector <4 x i32> %21, <4 x i32> poison, <4 x i32> zeroinitializer
  %23 = or <4 x i32> %22, <i32 0, i32 1, i32 2, i32 3>
  %24 = icmp ugt <4 x i32> %23, %11
  %25 = select <4 x i1> %24, <4 x i32> %14, <4 x i32> %16
  %26 = call i32 @llvm.vector.reduce.mul.v4i32(<4 x i32> %25)
  br label %27

27:                                               ; preds = %20, %0
  %28 = phi i32 [ 1, %0 ], [ %26, %20 ]
  %29 = call i32 (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i64 0, i64 0), i32 noundef %28) #6
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %2) #5
  ret i32 0
}

; Function Attrs: argmemonly mustprogress nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #2

; Function Attrs: nofree nounwind optsize
declare noundef i32 @__isoc99_scanf(i8* nocapture noundef readonly, ...) local_unnamed_addr #3

; Function Attrs: nofree nounwind optsize
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #3

; Function Attrs: argmemonly mustprogress nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #2

; Function Attrs: nofree nosync nounwind readnone willreturn
declare i32 @llvm.vector.reduce.mul.v4i32(<4 x i32>) #4

attributes #0 = { nofree norecurse nosync nounwind optsize readnone uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nounwind optsize uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { argmemonly mustprogress nofree nosync nounwind willreturn }
attributes #3 = { nofree nounwind optsize "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nofree nosync nounwind readnone willreturn }
attributes #5 = { nounwind }
attributes #6 = { optsize }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{!"Ubuntu clang version 14.0.0-1ubuntu1"}
!5 = distinct !{!5, !6, !7}
!6 = !{!"llvm.loop.mustprogress"}
!7 = !{!"llvm.loop.isvectorized", i32 1}
!8 = !{!9, !9, i64 0}
!9 = !{!"int", !10, i64 0}
!10 = !{!"omnipotent char", !11, i64 0}
!11 = !{!"Simple C/C++ TBAA"}
!12 = distinct !{!12, !6, !7}
