package org.undalcore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class UndalQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File undalDir = new File(getFilesDir().getAbsolutePath() + "/.undal");
        if (!undalDir.exists()) {
            undalDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
