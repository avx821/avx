/* LCM type definition class file
 * This file was automatically generated by lcm-gen
 * DO NOT MODIFY BY HAND!!!!
 */

package avionics;
 
import java.io.*;
import java.util.*;
import lcm.lcm.*;
 
public final class navio_imu_t implements lcm.lcm.LCMEncodable
{
    public long timestamp;
    public double imu_pos[];
    public double imu_vel[];
    public double imu_acc[];
 
    public navio_imu_t()
    {
        imu_pos = new double[3];
        imu_vel = new double[3];
        imu_acc = new double[3];
    }
 
    public static final long LCM_FINGERPRINT;
    public static final long LCM_FINGERPRINT_BASE = 0xca3ebaa28d24db34L;
 
    static {
        LCM_FINGERPRINT = _hashRecursive(new ArrayList<Class<?>>());
    }
 
    public static long _hashRecursive(ArrayList<Class<?>> classes)
    {
        if (classes.contains(avionics.navio_imu_t.class))
            return 0L;
 
        classes.add(avionics.navio_imu_t.class);
        long hash = LCM_FINGERPRINT_BASE
            ;
        classes.remove(classes.size() - 1);
        return (hash<<1) + ((hash>>63)&1);
    }
 
    public void encode(DataOutput outs) throws IOException
    {
        outs.writeLong(LCM_FINGERPRINT);
        _encodeRecursive(outs);
    }
 
    public void _encodeRecursive(DataOutput outs) throws IOException
    {
        outs.writeLong(this.timestamp); 
 
        for (int a = 0; a < 3; a++) {
            outs.writeDouble(this.imu_pos[a]); 
        }
 
        for (int a = 0; a < 3; a++) {
            outs.writeDouble(this.imu_vel[a]); 
        }
 
        for (int a = 0; a < 3; a++) {
            outs.writeDouble(this.imu_acc[a]); 
        }
 
    }
 
    public navio_imu_t(byte[] data) throws IOException
    {
        this(new LCMDataInputStream(data));
    }
 
    public navio_imu_t(DataInput ins) throws IOException
    {
        if (ins.readLong() != LCM_FINGERPRINT)
            throw new IOException("LCM Decode error: bad fingerprint");
 
        _decodeRecursive(ins);
    }
 
    public static avionics.navio_imu_t _decodeRecursiveFactory(DataInput ins) throws IOException
    {
        avionics.navio_imu_t o = new avionics.navio_imu_t();
        o._decodeRecursive(ins);
        return o;
    }
 
    public void _decodeRecursive(DataInput ins) throws IOException
    {
        this.timestamp = ins.readLong();
 
        this.imu_pos = new double[(int) 3];
        for (int a = 0; a < 3; a++) {
            this.imu_pos[a] = ins.readDouble();
        }
 
        this.imu_vel = new double[(int) 3];
        for (int a = 0; a < 3; a++) {
            this.imu_vel[a] = ins.readDouble();
        }
 
        this.imu_acc = new double[(int) 3];
        for (int a = 0; a < 3; a++) {
            this.imu_acc[a] = ins.readDouble();
        }
 
    }
 
    public avionics.navio_imu_t copy()
    {
        avionics.navio_imu_t outobj = new avionics.navio_imu_t();
        outobj.timestamp = this.timestamp;
 
        outobj.imu_pos = new double[(int) 3];
        System.arraycopy(this.imu_pos, 0, outobj.imu_pos, 0, 3); 
        outobj.imu_vel = new double[(int) 3];
        System.arraycopy(this.imu_vel, 0, outobj.imu_vel, 0, 3); 
        outobj.imu_acc = new double[(int) 3];
        System.arraycopy(this.imu_acc, 0, outobj.imu_acc, 0, 3); 
        return outobj;
    }
 
}

