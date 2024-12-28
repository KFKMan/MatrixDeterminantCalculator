function Test-MatrixDeterminant {
    param (
        [int]$size,
        [double[][]]$matrix,
        [double]$expectedDet
    )

    $filePath = Join-Path (Get-Location).Path "build\Debug\MatrixDeterminantCalculator.exe"

     $processInfo = New-Object System.Diagnostics.ProcessStartInfo
     $processInfo.FileName = $filePath
     $processInfo.RedirectStandardInput = $true
     $processInfo.RedirectStandardOutput = $true
     $processInfo.UseShellExecute = $false
     $processInfo.CreateNoWindow = $false
 
     $process = New-Object System.Diagnostics.Process
     $process.StartInfo = $processInfo
 
     #Write-Host "Process Started"
     $process.Start() | Out-Null
     #$process.BeginOutputReadLine()
     
     $process.StandardInput.WriteLine("$size")
     $process.StandardInput.WriteLine("$size")

     for ($row = 0; $row -lt $size; $row++) {
        for ($col = 0; $col -lt $size; $col++) {
            #Write-Host "Sending value: $($matrix[$row][$col])"
            $process.StandardInput.WriteLine($matrix[$row][$col])
        }
    }

    $process.StandardInput.WriteLine("0")

     $process.WaitForExit() | Out-Null

     $output = $process.StandardOutput.ReadToEnd()
     Write-Host "Output: $output"
 


     if($output -match "Determinant of the matrix is (\-?\d+(\.\d+)?)"){
        $resultDet = [double]$matches[1]
        if ($resultDet -eq $expectedDet) {
            Write-Host "Test passed. Expected determinant: $expectedDet, Calculated determinant: $resultDet" -ForegroundColor Green
            return $true
        } else {
            Write-Host "Test failed. Expected determinant: $expectedDet, Calculated determinant: $resultDet" -ForegroundColor Red
        }
     }
     else{
        Write-Host "Test failed. App didn't returned a result." -ForegroundColor Red
     }
     #$process.Kill() | Out-Null

     return $false
}

# Test Dataset
$tests = @(
    @{
        size = 2
        matrix = @(
            @(2, 3),
            @(1, 4)
        )
        expectedDet = 5
    },
    @{
        size = 2
        matrix = @(
            @(4, 7),
            @(2, 6)
        )
        expectedDet = 10
    },
    @{
        size = 2
        matrix = @(
            @(5, 1),
            @(3, 2)
        )
        expectedDet = 7
    },
    @{
        size = 3
        matrix = @(
            @(1, 2, 3),
            @(4, 5, 6),
            @(7, 8, 9)
        )
        expectedDet = 0
    },
    @{
        size = 3
        matrix = @(
            @(3, 2, 1),
            @(6, 5, 4),
            @(9, 8, 7)
        )
        expectedDet = 0
    },
    @{
        size = 3
        matrix = @(
            @(1, 0, 0),
            @(0, 3, 0),
            @(0, 0, 5)
        )
        expectedDet = 15
    },
    @{
        size = 2
        matrix = @(
            @(2, 1),
            @(1, 2)
        )
        expectedDet = 3
    },
    @{
        size = 2
        matrix = @(
            @(5, 7),
            @(3, 8)
        )
        expectedDet = 19
    },
    @{
        size = 3
        matrix = @(
            @(1, 2, 1),
            @(3, 4, 2),
            @(5, 6, 3)
        )
        expectedDet = 0
    },
    @{
        size = 3
        matrix = @(
            @(1, 1, 1),
            @(2, 2, 2),
            @(3, 3, 3)
        )
        expectedDet = 0
    },
    @{
        size = 4
        matrix = @(
            @(1, 2, 3, 4),
            @(5, 6, 7, 8),
            @(9, 10, 11, 12),
            @(13, 14, 15, 16)
        )
        expectedDet = 0
    },
    @{
        size = 4
        matrix = @(
            @(2, 4, 1, 3),
            @(1, 3, 2, 4),
            @(3, 2, 4, 1),
            @(4, 1, 3, 2)
        )
        expectedDet = 80
    },
    @{
        size = 5
        matrix = @(
            @(1, 2, 3, 4, 5),
            @(6, 7, 8, 9, 10),
            @(11, 12, 13, 14, 15),
            @(16, 17, 18, 19, 20),
            @(21, 22, 23, 24, 25)
        )
        expectedDet = 0
    },
    @{
        size = 5
        matrix = @(
            @(3, 1, 4, 1, 5),
            @(9, 2, 6, 5, 3),
            @(5, 8, 9, 7, 9),
            @(3, 4, 2, 5, 6),
            @(7, 1, 6, 8, 2)
        )
        expectedDet = -4115
    },
    @{
        size = 6
        matrix = @(
            @(1, 1, 1, 1, 1, 1),
            @(1, 2, 2, 2, 2, 2),
            @(1, 2, 3, 3, 3, 3),
            @(1, 2, 3, 4, 4, 4),
            @(1, 2, 3, 4, 5, 5),
            @(1, 2, 3, 4, 5, 6)
        )
        expectedDet = 1
    },
    @{
        size = 6
        matrix = @(
            @(2, 3, 1, 4, 5, 6),
            @(7, 8, 9, 1, 2, 3),
            @(4, 5, 6, 7, 8, 9),
            @(3, 1, 2, 8, 6, 4),
            @(1, 2, 3, 4, 5, 6),
            @(7, 8, 9, 1, 2, 3)
        )
        expectedDet = 0
    },
    @{
        size = 2
        matrix = @(
            @(1, 1),
            @(1, 1)
        )
        expectedDet = 0
    },
    @{
        size = 3
        matrix = @(
            @(1, 1, 1),
            @(1, 1, 1),
            @(1, 1, 1)
        )
        expectedDet = 0
    },
    @{
        size = 4
        matrix = @(
            @(1, 1, 1, 1),
            @(2, 2, 2, 2),
            @(3, 3, 3, 3),
            @(4, 4, 4, 4)
        )
        expectedDet = 0
    },
    @{
        size = 3
        matrix = @(
            @(1, 2, 3),
            @(4, 5, 6),
            @(7, 8, 9)
        )
        expectedDet = 0
    },
    @{
        size = 4
        matrix = @(
            @(1, 2, 3, 4),
            @(1, 2, 3, 5),
            @(1, 2, 3, 6),
            @(1, 2, 3, 7)
        )
        expectedDet = 0
    },
    @{
        size = 5
        matrix = @(
            @(5, 1, 2, 3, 4),
            @(0, 6, 7, 8, 9),
            @(1, 0, 6, 7, 8),
            @(2, 1, 0, 6, 7),
            @(3, 2, 1, 0, 6)
        )
        expectedDet = 5625
    },
    @{
        size = 3
        matrix = @(
            @(0, 10, 20),
            @(30, 0, 40),
            @(50, 60, 0)
        )
        expectedDet = 56000
    }
)

#Write-Host (Get-Location)
$successCount = 0
$errorCount = 0
foreach ($test in $tests) {
    if(Test-MatrixDeterminant -size $test.size -matrix $test.matrix -expectedDet $test.expectedDet){
        $successCount += 1
    }
    else {
        $errorCount += 1
    }
}

Write-Host "Success Count $successCount" -ForegroundColor Green
Write-Host "Error Count $errorCount" -ForegroundColor Red